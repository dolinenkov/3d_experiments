#pragma once
#include "config.hh"
#include "VertexFormat.hh"


struct StaticMesh
{
    StaticMesh();
    ~StaticMesh();

    void init(const Vertice & firstVertex, size_t vertexCount, const GLuint & firstIndex, size_t indexCount);

    void free();

    void draw();

    GLsizei indexCount;
    GLuint  indexData;
    GLuint  vertexData;
};


class Model
{
private:

public:
    void loadFromFile(const char * filename);

    void draw();

private:
    vector<size_t>      meshDrawOrder;
    vector<StaticMesh>  meshes;
};
