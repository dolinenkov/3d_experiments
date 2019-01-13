#pragma once
#include "config.hh"
#include "VertexFormat.hh"


class Transformation
{
public:
    Transformation();

    void setPosition(vec3 position);

    void setScale(vec3 scale);

    const mat4 & getModelMatrix() const;

private:
    bool         dirty;
    vec3         position;
    vec3         scale;
    mutable mat4 modelMatrix;
};


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


class Model : public Transformation
{
private:

public:
    void loadFromFile(const char * filename);

    void draw();

private:
    vector<size_t>      meshDrawOrder;
    vector<StaticMesh>  meshes;
};
