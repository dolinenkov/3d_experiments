#pragma once
#include <xrndr/config.hh>
#include <xrndr/detail/Stack.hh>

namespace xrndr
{

class MatrixStack
{
public:
    struct Settings
    {
        static Settings defaults();

        size_t projectionStackSize;
        size_t viewStackSize;
        size_t modelStackSize;
    };

    struct Cache
    {
        mat4 model;
        mat4 view;
        mat4 projection;
        mat4 modelView;
        mat4 viewProjection;
        mat4 modelViewProjection;
        mat3 normal;
    };

    MatrixStack();

    explicit MatrixStack(const Settings & settings);

    void pushProjection(const mat4 & projection);
    void pushView(const mat4 & view);
    void pushModel(const mat4 & model, bool additive);

    void popProjection();
    void popView();
    void popModel();

    const Cache & getCache();

private:
    Stack<mat4> _projection;
    Stack<mat4> _view;
    Stack<mat4> _model;
    Cache       _cache;
    bool        _isDirty;
};

}
