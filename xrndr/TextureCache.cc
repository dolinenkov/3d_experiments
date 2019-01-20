#include <xrndr/TextureCache.hh>

namespace xrndr
{

void TextureCache::clearTextures2D()
{
    _cached2dTextures.clear();
}

shared_ptr<Texture2D> TextureCache::findTexture2D(const Path & path)
{
    auto it = _cached2dTextures.find(path);
    return (it != _cached2dTextures.end()) ? it->second : nullptr;
}

void TextureCache::insertTexture2D(const Path & path, shared_ptr<Texture2D> texture)
{
    _cached2dTextures[path] = texture;
}

void TextureCache::removeTexture2D(const Path & path)
{
    _cached2dTextures.erase(path);
}

}