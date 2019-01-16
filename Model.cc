#include "Model.hh"


Transformation::Transformation()
    : _dirty(true)
    , _position(0.0f, 0.0f, 0.0f)
    , _scale(1.0f, 1.0f, 1.0f)
{
}

void Transformation::setPosition(vec3 position)
{
    if (_position != position)
    {
        _dirty = true;
        _position = position;
    }
}

void Transformation::setScale(vec3 scale)
{
    if (_scale != scale)
    {
        _dirty = true;
        _scale = scale;
    }
}

const mat4 & Transformation::getModelMatrix()
{
    if (_dirty)
    {
        _dirty = false;
        // Transform, rotate, scale
        _modelMatrix = ::glm::scale(translate(mat4(), _position), _scale);
    }

    return _modelMatrix;
}


//


Mesh::Mesh()
    : _vertexData(0)
    , _indexData(0)
    , _indexCount(0)
{
}

Mesh::~Mesh()
{
    free();
}

void Mesh::init(const Vertice * firstVertex, size_t vertexCount, const GLuint * firstIndex, size_t indexCount)
{
    free();

    _indexCount = indexCount;
    glGenBuffers(1, &_vertexData); gl_bugcheck();
    glGenBuffers(1, &_indexData); gl_bugcheck();

    glBindBuffer(GL_ARRAY_BUFFER, _vertexData); gl_bugcheck();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexData); gl_bugcheck();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * vertexCount, firstVertex, GL_STATIC_DRAW); gl_bugcheck();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, firstIndex, GL_STATIC_DRAW); gl_bugcheck();

}

void Mesh::free()
{
    if (_vertexData > 0)
    {
        glDeleteBuffers(1, &_vertexData); gl_bugcheck();
    }
    if (_indexData > 0)
    {
        glDeleteBuffers(1, &_indexData); gl_bugcheck();
    }
}

void Mesh::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexData); gl_bugcheck();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexData); gl_bugcheck();

    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr); gl_bugcheck();

    glBindBuffer(GL_ARRAY_BUFFER, 0); gl_bugcheck();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); gl_bugcheck();
}


//


Assimp::Importer g_Importer;


void Model::loadFromFile(const char * filename)
{
#ifdef _DEBUG
    g_Importer.SetExtraVerbose(true);
#endif

    auto scene = g_Importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (scene && scene->mRootNode)
    {
        assert((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == 0);

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

            meshes[i].init(vertices.data(), vertices.size(), indices.data(), indices.size());
        }

        _meshes = move(meshes);

        vector<size_t> meshDrawOrder(_countChildrenMeshes(scene->mRootNode));
        size_t index = 0;
        _assignMesh(meshDrawOrder, index, scene->mRootNode);

        _meshDrawOrder = move(meshDrawOrder);
    }

    auto err = g_Importer.GetErrorString();
    if (err && strlen(err) > 0)
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "assimp: %s\n", err);

    g_Importer.FreeScene();
}

void Model::draw()
{
    for (size_t index : _meshDrawOrder)
        _meshes[index].draw();
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

void Model::_assignMesh(vector<size_t> & meshDrawOrder, size_t & index, const aiNode * node) const
{
    if (node)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
            meshDrawOrder[index++] = node->mMeshes[i];

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            _assignMesh(meshDrawOrder, index, node->mChildren[i]);
    }
}

//

void Plane::init()
{
    Vertice vertex[6] = 
    {
        { {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f } },
        { { 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f } },
        { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f } },
        { { 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f } },
        /*
        */
        { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f } },
        { { 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f } }
    };

    GLuint index[] =
    {
        0, 1, 2,
        1, 2, 3
    };

    mesh.init(vertex, 4, index, 6);
}

void Plane::draw()
{
    mesh.draw();
}
