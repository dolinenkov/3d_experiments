#pragma once
#include "config.hh"
#include "VertexFormat.hh"
#include "Mesh.hh"


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


class Model : public Transformation
{
public:
    void loadFromFile(const char * filename, const VerticeFormat & verticeFormat);

    void draw();

private:
    unsigned int _countChildrenMeshes(const aiNode * node) const;

    void _assignMesh(vector<size_t> & meshDrawOrder, size_t & index, const aiNode * node) const;

private:
    vector<Mesh>   _meshes;
    vector<size_t> _meshDrawOrder;
};
