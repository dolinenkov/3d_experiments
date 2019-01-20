#pragma once
#include <xrndr/Texture2D.hh>
#include <xrndr/TextureCache.hh>

namespace xrndr
{

class Texture2DLoader
{
public:
    Texture2DLoader();

    void setTextureCache(TextureCache * textureCache);

    void setFilename(const Path & path);

    void setIsSRGB(bool isSRGB);

    shared_ptr<Texture2D> load();

    bool _loadFile();

    bool _decode();

    GLuint _upload();

private:
    TextureCache *  _textureCache;

    Path            _path;
    bool            _isSRGB;

    vector<char>    _rawData;
    vector<char>    _decodedData;
    uint32_t        _width;
    uint32_t        _height;
};

}
