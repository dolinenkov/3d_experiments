#pragma once
#include "config.hh"


class Projection
{
public:
    Projection();

    // TODO: ensure it's really working
    void makeOrtho(float left, float right, float bottom, float top);

    void makePerspective(float fovy, float ar, float znear, float zfar);

    const mat4 & getProjectionMatrix();

private:
    bool _isDirty;
    mat4 _projectionMatrix;
    bool _isPerspective;
    union
    {
        struct
        {
            float fovy;
            float ar;
            float znear;
            float zfar;
        } perspective;
        struct
        {
            float left;
            float right;
            float bottom;
            float top;
        } ortho;
    };
};