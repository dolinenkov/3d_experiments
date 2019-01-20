#pragma once
#include "config.hh"
#include "Texture2D.hh"

namespace xrndr
{

class TextureCache
{
public:

    void clearTextures2D();

    shared_ptr<Texture2D> findTexture2D(const Path & path);

    void insertTexture2D(const Path & path, shared_ptr<Texture2D> texture);

    void removeTexture2D(const Path & path);


private:
    map<Path, shared_ptr<Texture2D>> _cached2dTextures;

};


}