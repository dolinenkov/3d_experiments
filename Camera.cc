#include "Camera.hh"


Camera::Camera()
    : _eulerAngles(0.0f, 0.0f)
    , _upVector(0.0f, 1.0f, 0.0f)
    , _frontDirection(_getDirectionFromEulerAngles(_eulerAngles))
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

void Camera::setEulerAngles(float pitch, float yaw)
{
    if (pitch != _eulerAngles[0] || yaw != _eulerAngles[1])
    {
        _needUpdateViewMatrix = true;
        _eulerAngles = { pitch, yaw };
        _frontDirection = _getDirectionFromEulerAngles(_eulerAngles);
        _rightDirection = cross(_frontDirection, _upDirection);
    }
}

void Camera::setFrontVector(const vec3 & frontVector)
{
    const vec2 eulerAngles = _getEulerAnglesFromDirection(frontVector);
    setEulerAngles(eulerAngles[0], eulerAngles[1]);
}

const vec3 & Camera::getFrontVector() const
{
    return _frontDirection;
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

void Camera::turnRight(float angle)
{
    setEulerAngles(_eulerAngles[0], _eulerAngles[1] + angle);
}

void Camera::turnUp(float angle)
{
    setEulerAngles(clamp(_eulerAngles[0] + angle, -half_pi<float>(), half_pi<float>()), _eulerAngles[1]);
}

const mat4 & Camera::getViewMatrix()
{
    if (_needUpdateViewMatrix)
    {
        _needUpdateViewMatrix = false;
        _viewMatrix = lookAt(_position, _position + _frontDirection, _upVector);
    }
    return _viewMatrix;
}

vec2 Camera::_getEulerAnglesFromDirection(const vec3 & direction)
{
    const vec3 ndirection = length(direction) > 0.0f ? normalize(direction) : direction;

    float pitch;
    if (ndirection.y >= 1.0f)
        pitch = glm::half_pi<float>();
    else if (ndirection.y <= -1.0f)
        pitch = -glm::half_pi<float>();
    else
        pitch = asin(ndirection.y);

    float yaw = ndirection.z / cos(pitch);
    if (yaw >= 1.0f)
        yaw = glm::half_pi<float>();
    else if (yaw <= 1.0f)
        yaw = -glm::half_pi<float>();
    else
        yaw = asin(yaw);

    return { pitch, yaw };
}

vec3 Camera::_getDirectionFromEulerAngles(const vec2 & _eulerAngles)
{
    const float x = cos(_eulerAngles[0]) * cos(_eulerAngles[1]);
    const float y = sin(_eulerAngles[0]);
    const float z = cos(_eulerAngles[0]) * sin(_eulerAngles[1]);
    return normalize(vec3(x, y, z));
}
