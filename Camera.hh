#pragma once
#include "config.hh"

class Camera
{
public:
    Camera();

    void setPosition(const vec3 & position);
    const vec3 & getPosition() const;

    void setFrontVector(const vec3 & frontVector);
    const vec3 & getFrontVector() const;

    void setUpVector(const vec3 & upVector);
    const vec3 & getUpVector() const;

    const vec3 & getFrontDirection() const;
    const vec3 & getUpDirection() const;
    const vec3 & getRightDirection() const;

    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);

    const mat4 & getViewMatrix();

private:
    void update();

private:
    vec3 _position;

    vec3 _frontVector;
    vec3 _upVector;

    vec3 _frontDirection;
    vec3 _upDirection;
    vec3 _rightDirection;

    bool _needUpdateViewMatrix;
    mat4 _viewMatrix;
};
