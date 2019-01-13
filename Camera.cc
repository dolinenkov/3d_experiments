#include "Camera.hh"

Camera::Camera()
    : _frontVector(0.0f, 0.0f, -1.0f)
    , _upVector(0.0f, 1.0f, 0.0f)
    , _frontDirection(_frontVector)
    , _upDirection(_upVector)
    , _rightDirection(cross(_frontDirection, _upDirection))
    , _needUpdateViewMatrix(true)
{
}

void Camera::setPosition(const vec3 & position)
{
    if (_position != position)
    {
        _needUpdateViewMatrix = true;
        _position = position;
    }
}

const vec3 & Camera::getPosition() const
{
    return _position;
}

void Camera::setFrontVector(const vec3 & frontVector)
{
    if (_frontVector != frontVector)
    {
        _needUpdateViewMatrix = true;
        _frontVector = frontVector;
        _frontDirection = normalize(_frontVector);
        _rightDirection = cross(_frontDirection, _upDirection);
    }
}

const vec3 & Camera::getFrontVector() const
{
    return _frontVector;
}

void Camera::setUpVector(const vec3 & upVector)
{
    if (_upVector != upVector)
    {
        _needUpdateViewMatrix = true;
        _upVector = upVector;
        _upDirection = normalize(_upVector);
        _rightDirection = cross(_frontDirection, _upDirection);
    }
}

const vec3 & Camera::getUpVector() const
{
    return _upVector;
}

const vec3 & Camera::getFrontDirection() const
{
    return _frontDirection;
}

const vec3 & Camera::getUpDirection() const
{
    return _upDirection;
}

const vec3 & Camera::getRightDirection() const
{
    return _rightDirection;
}

void Camera::moveForward(float distance)
{
    if (distance != 0.0f)
        setPosition(getPosition() + getFrontDirection() * distance);
}

void Camera::moveRight(float distance)
{
    if (distance != 0.0f)
        setPosition(getPosition() + getRightDirection() * distance);
}

void Camera::moveUp(float distance)
{
    if (distance != 0.0f)
        setPosition(getPosition() + getUpDirection() * distance);
}

const mat4 & Camera::getViewMatrix()
{
    if (_needUpdateViewMatrix)
    {
        _needUpdateViewMatrix = false;
        _viewMatrix = lookAt(_position, _position + _frontVector, _upVector);
    }
    return _viewMatrix;
}
