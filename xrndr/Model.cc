#include <xrndr/Model.hh>
#include <xrndr/Scene.hh>
#include <xrndr/json.hh>


namespace xrndr
{

Transformation::Transformation()
    : _dirty(true)
    , _position(0.0f, 0.0f, 0.0f)
    , _scale(1.0f, 1.0f, 1.0f)
{
}

void Transformation::setPosition(const vec3 & position)
{
    if (_position != position)
    {
        _dirty = true;
        _position = position;
    }
}

void Transformation::setScale(const vec3 & scale)
{
    if (_scale != scale)
    {
        _dirty = true;
        _scale = scale;
    }
}

void Transformation::setRotation(const vec3 & angles)
{
    if (_angles != angles)
    {
        _dirty = true;
        _angles = angles;
    }
}

const mat4 & Transformation::getModelMatrix()
{
    if (_dirty)
    {
        _dirty = false;
        // Transform, rotate, scale
        _modelMatrix = ::glm::scale(yawPitchRoll(_angles[0], _angles[1], _angles[2]) * translate(mat4(1.0), _position), _scale);
    }

    return _modelMatrix;
}


//


Assimp::Importer g_Importer;


void Model::loadFromFile(const char * filename, const VerticeFormat & verticeFormat)
{
    auto scene = g_Importer.ReadFile(format("resources/models/{}", filename), aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
    if (scene && scene->mRootNode)
    {
        assert((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0);

        // materials

        vector<Material> materials(scene->mNumMaterials);

        for (unsigned i = 0; i < scene->mNumMaterials; ++i)
        {
            const auto material = scene->mMaterials[i];

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

                std::string file(path.C_Str());
                const size_t dotPos = file.rfind('.');
                file = file.substr(0, dotPos) + ".json";

                std::vector<char> materialJson;

                if (auto f = SDL_RWFromFile(format("resources/materials/{}", file).c_str(), "r"))
                {
                    auto s = SDL_RWsize(f);
                    materialJson.resize(s);
                    SDL_RWread(f, materialJson.data(), materialJson.size(), 1);
                    SDL_RWclose(f);
                }

                json::Object materialObject(materialJson.data(), materialJson.size());
                if (!materialObject.empty())
                {
                    materials[i].albedo = Texture2D::Loader().load(Texture2D::Description{ materialObject.getString("albedo"), true });

                    materials[i].normal = Texture2D::Loader().load(Texture2D::Description{ materialObject.getString("normal"), false});

                    materials[i].metalnessRoughnessAO = Texture2D::Loader().load(Texture2D::Description{ materialObject.getString("metalness_roughness_ao"), false});
                }
            }

            //if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
            //{
            //    aiString path;
            //    material->GetTexture(aiTextureType_SPECULAR, 0, &path);

            //    textureLoader.setFilename(path.C_Str());
            //    //materials[i].specularTexture = textureLoader.load();
            //}

            //material->Get(AI_MATKEY_SHININESS, materials[i].shininess);
        }

        // meshes

        vector<Mesh> meshes(scene->mNumMeshes);

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            const auto mesh = scene->mMeshes[i];

            vector<Vertice> vertices(mesh->mNumVertices);

            for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
            {
                vertices[j].position[0] = mesh->mVertices[j].x;
                vertices[j].position[1] = mesh->mVertices[j].y;
                vertices[j].position[2] = mesh->mVertices[j].z;

                assert(mesh->HasNormals());
                if (mesh->HasNormals())
                {
                    vertices[j].normal[0] = mesh->mNormals[j].x;
                    vertices[j].normal[1] = mesh->mNormals[j].y;
                    vertices[j].normal[2] = mesh->mNormals[j].z;
                }

                assert(mesh->HasTextureCoords(0));
                if (mesh->HasTextureCoords(0))
                {
                    vertices[j].texture[0] = mesh->mTextureCoords[0][j].x;
                    vertices[j].texture[1] = mesh->mTextureCoords[0][j].y;
                    vertices[j].texture[2] = mesh->mTextureCoords[0][j].z;
                }
            }

            size_t indexCount = 0;
            for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
                indexCount += mesh->mFaces[j].mNumIndices;

            vector<GLuint> indices(indexCount);
            indexCount = 0;

            for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
                for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; ++k)
                    indices[indexCount++] = mesh->mFaces[j].mIndices[k];

            MeshData meshData;
            meshData.vertexData = vertices.data();
            meshData.vertexSize = vertices.size();
            meshData.indexData = indices.data();
            meshData.indexSize = indices.size();
            meshData.format = &verticeFormat;

            meshes[i].init(meshData);
        }


        vector<Node> order(_countChildrenMeshes(scene->mRootNode));
        size_t index = 0;
        _assignMesh(order, index, scene->mRootNode);

        for (Node & node : order)
            node.material = scene->mMeshes[node.mesh]->mMaterialIndex;

        _materials = move(materials);
        _meshes = move(meshes);
        _order = move(order);
    }

    auto err = g_Importer.GetErrorString();
    if (err && strlen(err) > 0)
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "assimp: %s\n", err);

    g_Importer.FreeScene();
}

void Model::draw(Renderer & renderer)
{
    auto is_ready = [](std::shared_ptr<Texture2D> t)
    {
        return t && t->is_ready();
    };

    for (const auto& node : _order)
    {
        const auto& mat =_materials[node.material];
        if (!is_ready(mat.albedo) || !is_ready(mat.normal) || !is_ready(mat.metalnessRoughnessAO))
        {
            return;
        }
    }

    size_t material = (size_t) -1;

    for (const Node & node : _order)
    {
        if (node.material != material)
        {
            // use material
            material = node.material;

            renderer.setMaterial(_materials.data() + material);
        }

        _meshes[node.mesh].draw();
    }
}

unsigned int Model::_countChildrenMeshes(const aiNode * node) const
{
    unsigned int childrenCount = 0;
    if (node)
    {
        childrenCount += node->mNumMeshes;

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            childrenCount += _countChildrenMeshes(node->mChildren[i]);
    }

    return childrenCount;
}

void Model::_assignMesh(vector<Node> & order, size_t & index, const aiNode * node) const
{
    if (node)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
            order[index++].mesh = node->mMeshes[i];

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            _assignMesh(order, index, node->mChildren[i]);
    }
}

}