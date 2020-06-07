#pragma once
#include <xrndr/config.hh>
#include <xrndr/VertexFormat.hh>
#include <xrndr/Mesh.hh>
#include <xrndr/Material.hh>
#include <xrndr/Texture2DLoader.hh>


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

class Renderer;

class Model : public Transformation
{
private:
    struct Node
    {
        size_t mesh;
        size_t material;
    };

public:
    void loadFromFile(const char * filename, const VerticeFormat & verticeFormat);

    void draw(Renderer & renderer);

private:
    unsigned int _countChildrenMeshes(const aiNode * node) const;

    void _assignMesh(vector<Node> & meshDrawOrder, size_t & index, const aiNode * node) const;

private:
    vector<Mesh>     _meshes;
    vector<Material> _materials;
    vector<Node>     _order;
};

}
