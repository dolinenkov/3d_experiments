#pragma once
#include "config.hh"
#include "VertexFormat.hh"


class Transformation
{
public:
    Transformation();

    void setPosition(vec3 position);

    void setScale(vec3 scale);

    const mat4 & getModelMatrix();

private:
    bool _dirty;
    vec3 _position;
    vec3 _scale;
    mat4 _modelMatrix;
};


class Mesh
{
public:
    Mesh();
    ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh & operator=(const Mesh &) = delete;

    void init(const Vertice * firstVertex, size_t vertexCount, const GLuint * firstIndex, size_t indexCount);

    void free();

    void draw();

private:
    GLsizei _indexCount;
    GLuint  _indexData;
    GLuint  _vertexData;
};


class Model : public Transformation
{
public:
    void loadFromFile(const char * filename);

    void draw();

private:
    unsigned int _countChildrenMeshes(const aiNode * node) const;

    void _assignMesh(vector<size_t> & meshDrawOrder, size_t & index, const aiNode * node) const;

private:
    vector<Mesh>   _meshes;
    vector<size_t> _meshDrawOrder;
};


class Plane
{
public:
    void init();

    void draw();

private:
    Mesh mesh;
};