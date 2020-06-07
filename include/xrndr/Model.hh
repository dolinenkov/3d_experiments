#pragma once
#include <xrndr/config.hh>
#include <xrndr/VertexFormat.hh>
#include <xrndr/Mesh.hh>
#include <xrndr/Material.hh>
#include <xrndr/async/Async.hh>
#include <entt/entt.hpp>

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
    enum class State
    {
        Empty,
        Await,
        Ready,
        Error,
    };

    struct Description
    {
        Path path;
    };

    struct Loader : entt::loader<Loader, Model>
    {
        std::shared_ptr<Model> load(const Description&, const VerticeFormat&) const;
        std::shared_ptr<Model> load(const Description&, const VerticeFormat&, ExecutionContext&) const;

    };

    struct Cache : entt::cache<Model>
    {
        static Cache& instance();
    };

    bool is_ready() const;

    void draw(Renderer & renderer);

    void setNoTextures(bool);

private:
    unsigned int _countChildrenMeshes(const aiNode * node) const;

    void _assignMesh(vector<Node> & meshDrawOrder, size_t & index, const aiNode * node) const;

private:
    vector<Mesh> _meshes;
    vector<Material> _materials;
    vector<Node> _order;
    State _state = State::Empty;
    bool _notextures = false;
};

}
