#pragma once
#include <xrndr/config.hh>
#include <xrndr/Texture2D.hh>

namespace xrndr
{

struct Material
{
    shared_ptr<Texture2D> albedo; // 3
    shared_ptr<Texture2D> normal; // 3
    shared_ptr<Texture2D> metalness; // 1
    shared_ptr<Texture2D> ambientOcclusion; // ?
    shared_ptr<Texture2D> roughness; // 1
};

}