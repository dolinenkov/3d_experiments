#include "Model.hh"


Transformation::Transformation()
    : dirty(true)
{
}

void Transformation::setPosition(vec3 position)
{
    if (this->position != position)
    {
        dirty = true;
        this->position = position;
    }
}

void Transformation::setScale(vec3 scale)
{
    if (this->scale != scale)
    {
        dirty = true;
        this->scale = scale;
    }
}

const mat4 & Transformation::getModelMatrix() const
{
    if (dirty)
    {
        // Transform, rotate, scale
        modelMatrix = ::glm::scale(translate(mat4(), this->position), this->scale);
    }

    return modelMatrix;
}


//


StaticMesh::StaticMesh()
    : vertexData(0)
    , indexData(0)
    , indexCount(0)
{
}

StaticMesh::~StaticMesh()
{
    free();
}

void StaticMesh::init(const Vertice & firstVertex, size_t vertexCount, const GLuint & firstIndex, size_t indexCount)
{
    free();

    GLuint tmp[2];
    glGenBuffers(2, tmp); gl_bugcheck();

    vertexData = tmp[0];
    indexData = tmp[1];
    this->indexCount = indexCount;

    glBindBuffer(GL_ARRAY_BUFFER, vertexData); gl_bugcheck();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * vertexCount, addressof(firstVertex), GL_STATIC_DRAW); gl_bugcheck();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData); gl_bugcheck();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, addressof(firstIndex), GL_STATIC_DRAW); gl_bugcheck();
}

void StaticMesh::free()
{
    GLuint tmp[2] = { vertexData, indexData };
    glDeleteBuffers(2, tmp); gl_bugcheck();
}

void StaticMesh::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexData); gl_bugcheck();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexData); gl_bugcheck();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr); gl_bugcheck();
}


//


void Model::loadFromFile(const char * filename)
{
    Assimp::Importer importer;

    auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (scene && scene->mRootNode)
    {
        vector<StaticMesh> staticMeshes(scene->mNumMeshes);

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

            staticMeshes[i].init(vertices[0], vertices.size(), indices[0], indices.size());
        }

        this->meshes.swap(staticMeshes);
    }
}

void Model::draw()
{
    for (auto & mesh : meshes)
        mesh.draw();
}
