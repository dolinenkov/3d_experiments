#pragma once
#include <xrndr/config.hh>

namespace xrndr
{

struct PointLight
{
    vec3 position;
    vec3 color;
    vec3 attenuation;
    vec3 intensity;
};

struct DirectedLight
{
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec3 intensity;
};

}