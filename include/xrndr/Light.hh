#pragma once
#include <xrndr/config.hh>

namespace xrndr
{

struct Light
{
    vec3 position;
    vec3 color;
    vec3 attenuation;
    vec3 intensity;
};

}