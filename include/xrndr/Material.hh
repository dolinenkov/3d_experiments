#pragma once
#include <xrndr/config.hh>
#include <xrndr/Texture2D.hh>

namespace xrndr
{

struct Material
{
    shared_ptr<Texture2D> diffuseTexture;
};

}