#include <xrndr/Mesh.hh>

namespace xrndr
{

Mesh::Mesh()
    : _vertexArrayObject(0)
    , _vertexBufferObject(0)
    , _indexBufferObject(0)
    , _indexCount(0)
{
}

Mesh::~Mesh()
{
    free();
}

void Mesh::init(const MeshData & meshData)
{
    if (meshData.vertexData == nullptr ||
        meshData.vertexSize == 0 ||
        meshData.indexData == nullptr ||
        meshData.indexSize == 0)
    {
        assert(false);
        return;
    }

    free();

    glGenVertexArrays(1, &_vertexArrayObject); gl_bugcheck();
    glGenBuffers(1, &_vertexBufferObject); gl_bugcheck();
    glGenBuffers(1, &_indexBufferObject); gl_bugcheck();

    if (_vertexArrayObject == 0 ||
        _vertexBufferObject == 0 ||
        _indexBufferObject == 0)
    {
        free();
        assert(false);
        return;
    }

    glBindVertexArray(_vertexArrayObject); gl_bugcheck();

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); gl_bugcheck();
    glBufferData(GL_ARRAY_BUFFER, meshData.vertexSize * sizeof(Vertice), meshData.vertexData, GL_STATIC_DRAW); gl_bugcheck();


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject); gl_bugcheck();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indexSize * sizeof(uint32_t), meshData.indexData, GL_STATIC_DRAW); gl_bugcheck();

    _indexCount = meshData.indexSize;

    if (meshData.format.position != VerticeFormat::NOT_PRESENT)
    {
        glVertexAttribPointer(
            meshData.format.position,
            sizeof(Vertice::position) / sizeof(Vertice::position[0]),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertice),
            reinterpret_cast<const void *>(offsetof(Vertice, position))); gl_bugcheck();

        glEnableVertexAttribArray(meshData.format.position); gl_bugcheck();
    }

    if (meshData.format.texture != VerticeFormat::NOT_PRESENT)
    {
        glVertexAttribPointer(
            meshData.format.texture,
            sizeof(Vertice::texture) / sizeof(Vertice::texture[0]),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertice),
            reinterpret_cast<const void *>(offsetof(Vertice, texture))); gl_bugcheck();

        glEnableVertexAttribArray(meshData.format.texture); gl_bugcheck();
    }

    if (meshData.format.normal != VerticeFormat::NOT_PRESENT)
    {
        glVertexAttribPointer(
            meshData.format.normal,
            sizeof(Vertice::normal) / sizeof(Vertice::normal[0]),
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertice),
            reinterpret_cast<const void *>(offsetof(Vertice, normal))); gl_bugcheck();

        glEnableVertexAttribArray(meshData.format.normal); gl_bugcheck();
    }
}

void Mesh::free()
{
    _indexCount = 0;

    if (_vertexArrayObject > 0)
    {
        glDeleteVertexArrays(1, &_vertexArrayObject); gl_bugcheck();
    }

    if (_vertexBufferObject > 0)
    {
        glDeleteBuffers(1, &_vertexBufferObject); gl_bugcheck();
    }

    if (_indexBufferObject > 0)
    {
        glDeleteBuffers(1, &_indexBufferObject); gl_bugcheck();
    }
}

void Mesh::draw()
{
    if (_vertexArrayObject > 0 && _indexCount > 0)
    {
        glBindVertexArray(_vertexArrayObject); gl_bugcheck();

        // TODO: it should work without binding EBO, however, error is reported by RenderDoc - should check
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferObject);
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr); gl_bugcheck();
    }
}

}
