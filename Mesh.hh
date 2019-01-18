#pragma once
#include "config.hh"
#include "VertexFormat.hh"


struct MeshData
{
    const Vertice *         vertexData;
    size_t                  vertexSize;

    const uint32_t *        indexData;
    size_t                  indexSize;

    const VerticeFormat *   format;
};


class Mesh
{
public:
    Mesh();

    Mesh(const Mesh &) = delete;

    Mesh(Mesh &&) = default;

    Mesh & operator=(const Mesh &) = delete;

    Mesh & operator=(Mesh &&) = default;

    ~Mesh();

    void init(const MeshData & meshData);

    void free();

    void draw();

private:
    GLuint  _vertexArrayObject;
    GLuint  _vertexBufferObject;
    GLuint  _indexBufferObject;
    GLsizei _indexCount;
};
