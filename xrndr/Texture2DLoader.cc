#include <xrndr/Texture2DLoader.hh>

namespace xrndr
{

Texture2DLoader::Texture2DLoader()
    : _textureCache(nullptr)
    , _isSRGB(false)
    , _width(0)
    , _height(0)
{
}

void Texture2DLoader::setTextureCache(TextureCache * textureCache)
{
    _textureCache = textureCache;
}

void Texture2DLoader::setFilename(const Path & path)
{
    _path = path;
}

void Texture2DLoader::setIsSRGB(bool isSRGB)
{
    _isSRGB = isSRGB;
}

shared_ptr<Texture2D> Texture2DLoader::load()
{
    shared_ptr<Texture2D> texture;

    if (_loadFile())
    {
        if (_decode())
        {
            texture = make_shared<Texture2D>(_upload());

            if (_textureCache)
                _textureCache->insertTexture2D(_path, texture);

            _decodedData.clear();
        }
        _rawData.clear();
    }

    return texture;
}

bool Texture2DLoader::_loadFile()
{
    bool read = false;
    if (!_path.empty())
    {
        auto file = SDL_RWFromFile(_path.c_str(), "rb");
        if (file)
        {
            auto size = SDL_RWsize(file);
            _rawData.resize(size);
            SDL_RWread(file, _rawData.data(), _rawData.size(), 1);
            SDL_RWclose(file);
            read = true;
        }
    }
    return read;
}

bool Texture2DLoader::_decode()
{
    bool decoded = false;

    if (!_rawData.empty())
    {
        int width = 0;
        int height = 0;

        auto img = SOIL_load_image_from_memory(reinterpret_cast<const unsigned char *>(_rawData.data()), _rawData.size(), &width, &height, nullptr, SOIL_LOAD_RGBA);
        if (img)
        {
            _decodedData.resize(width * height * 4);
            memcpy(_decodedData.data(), img, _decodedData.size());
            SOIL_free_image_data(img);
        }

        _width = width;
        _height = height;
        decoded = true;
    }

    return decoded;
}

GLuint Texture2DLoader::_upload()
{
    GLuint texture = 0;
    if (_width > 0 && _height > 0 && !_decodedData.empty())
    {
        glGenTextures(1, &texture);
        if (texture > 0)
        {
            glBindTexture(GL_TEXTURE_2D, texture); gl_bugcheck();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); gl_bugcheck();

            glTexImage2D(GL_TEXTURE_2D, 0, _isSRGB ? GL_SRGB : GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _decodedData.data()); gl_bugcheck();
            glGenerateMipmap(GL_TEXTURE_2D); gl_bugcheck();
        }
    }
    return texture;
}

}
