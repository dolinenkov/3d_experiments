#pragma once
#include <xrndr/config.hh>
#include <xrndr/VertexFormat.hh>
#include <xrndr/Mesh.hh>


namespace xrndr
{

class Transformation
{
public:
    Transformation();

    void setPosition(const vec3 & position);

    void setScale(const vec3 & scale);

    void setRotation(const vec3 & angles);

    const mat4 & getModelMatrix();

private:
    bool _dirty;
    vec3 _position;
    vec3 _scale;
    vec3 _angles;
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

}
