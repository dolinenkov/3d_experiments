#include <xrndr/MatrixStack.hh>

namespace xrndr
{

MatrixStack::Settings MatrixStack::Settings::defaults()
{
    MatrixStack::Settings settings;
    settings.projectionStackSize = 5;
    settings.viewStackSize = 5;
    settings.modelStackSize = 100;
    return settings;
}


MatrixStack::MatrixStack()
    : MatrixStack(Settings::defaults())
{
}

MatrixStack::MatrixStack(const Settings & settings)
{
    _projection.reserve(settings.projectionStackSize);
    _view.reserve(settings.viewStackSize);
    _model.reserve(settings.modelStackSize);

    _isDirty = true;
}

void MatrixStack::pushProjection(const mat4 & projection)
{
    _projection.push(projection);
    _isDirty = true;
}

void MatrixStack::pushView(const mat4 & view)
{
    _view.push(view);
    _isDirty = true;
}

void MatrixStack::pushModel(const mat4 & model, bool additive)
{
    if (additive && !_model.empty())
        _model.push(_model.top() * model);
    else
        _model.push(model);

    _isDirty = true;
}

void MatrixStack::popProjection()
{
    _projection.pop();
    _isDirty = true;
}

void MatrixStack::popView()
{
    _view.pop();
    _isDirty = true;
}

void MatrixStack::popModel()
{
    _model.pop();
    _isDirty = true;
}

const MatrixStack::Cache & MatrixStack::getCache()
{
    if (_isDirty)
    {
        _isDirty = false;

        _cache.model                = _model.top();
        _cache.view                 = _view.top();
        _cache.projection           = _projection.top();

        _cache.modelView            = _cache.view * _cache.model;
        _cache.viewProjection       = _cache.projection * _cache.view;
        _cache.modelViewProjection  = _cache.projection * _cache.modelView;

        _cache.normal               = mat3(transpose(inverse(_cache.model)));
    }
    return _cache;
}

}
