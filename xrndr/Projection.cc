#include <xrndr/Projection.hh>

namespace xrndr
{

Projection::Projection()
    : _isDirty(false)
    , _isPerspective(false)
{
    ortho.left = 0.0f;
    ortho.right = 1.0f;
    ortho.bottom = 0.0f;
    ortho.top = 1.0f;
}

void Projection::makeOrtho(float left, float right, float bottom, float top)
{
    if (_isPerspective || ortho.left != left || ortho.right != right || ortho.bottom != bottom || ortho.top != top)
    {
        _isDirty = true;
        _isPerspective = false;
        ortho.left = left;
        ortho.right = right;
        ortho.bottom = bottom;
        ortho.top = top;
    }
}

void Projection::makePerspective(float fovy, float ar, float znear, float zfar)
{
    if (!_isPerspective || perspective.fovy != fovy || perspective.ar != ar || perspective.znear != znear || perspective.zfar != zfar)
    {
        _isDirty = true;
        _isPerspective = true;
        perspective.fovy = fovy;
        perspective.ar = ar;
        perspective.znear = znear;
        perspective.zfar = zfar;
    }
}

const mat4 & Projection::getProjectionMatrix()
{
    if (_isDirty)
    {
        _isDirty = false;

        if (_isPerspective)
            _projectionMatrix = glm::perspective(radians(perspective.fovy), perspective.ar, perspective.znear, perspective.zfar);
        else
            _projectionMatrix = glm::ortho(ortho.left, ortho.right, ortho.bottom, ortho.top);
    }

    return _projectionMatrix;
}

}
