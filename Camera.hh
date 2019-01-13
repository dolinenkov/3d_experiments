#pragma once
#include "config.hh"

/*
    TODO:
        fix gimbal lock and teleporting
        fix cases of normalization of zero length vectors
*/

class Camera
{
public:
    Camera();

    void setPosition(const vec3 & position);
    const vec3 & getPosition() const;

    void setEulerAngles(float pitch, float yaw);

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

    void turnRight(float angle);
    void turnUp(float angle);

    const mat4 & getViewMatrix();

private:
    static vec2 _getEulerAnglesFromDirection(const vec3 & direction);

    static vec3 _getDirectionFromEulerAngles(const vec2 & _eulerAngles);

    void update();

private:
    vec3 _position;

    vec2 _eulerAngles; // pitch, yaw
    vec3 _upVector;

    vec3 _frontDirection;
    vec3 _upDirection;
    vec3 _rightDirection;

    bool _needUpdateViewMatrix;
    mat4 _viewMatrix;
};
