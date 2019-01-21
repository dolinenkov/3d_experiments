#pragma once
#include <xrndr/config.hh>
#include <xrndr/Texture2D.hh>

namespace xrndr
{

struct Material
{
    float                   shininess;
    shared_ptr<Texture2D>   diffuseTexture;
    shared_ptr<Texture2D>   specularTexture;
};

}