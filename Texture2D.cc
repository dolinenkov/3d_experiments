#include "Texture2D.hh"


Texture2D::Texture2D(GLuint name) : name(name)
{
}

Texture2D::~Texture2D()
{
    if (name > 0)
    {
        glDeleteTextures(1, &name); gl_bugcheck();
    }
}

void Texture2D::use(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit); gl_bugcheck();
    glBindTexture(GL_TEXTURE_2D, name); gl_bugcheck();
}


Texture2DLoader::Texture2DLoader(): width(0), height(0), isSRGB(false)
{
}

void Texture2DLoader::setFilename(const char * filename)
{
    vector<char> data;
    if (_loadFile(filename, data))
        _decode(data);
}

void Texture2DLoader::setIsSRGB(bool isSRGB)
{
    this->isSRGB = isSRGB;
}

shared_ptr<Texture2D> Texture2DLoader::load()
{
    return make_shared<Texture2D>(_upload());
}

bool Texture2DLoader::_loadFile(const char * filename, vector<char> & data)
{
    bool read = false;
    auto file = SDL_RWFromFile(filename, "rb");
    if (file)
    {
        auto size = SDL_RWsize(file);
        data.resize(size);
        SDL_RWread(file, data.data(), data.size(), 1);
        SDL_RWclose(file);
        read = true;
    }
    return read;
}

void Texture2DLoader::_decode(const vector<char> & data)
{
    int w, h;
    auto img = SOIL_load_image_from_memory(reinterpret_cast<const unsigned char *>(data.data()), data.size(), &w, &h, nullptr, SOIL_LOAD_RGBA);
    if (img)
    {
        width = w;
        height = h;
        this->data.resize(w * h * 4);
        memcpy(this->data.data(), img, this->data.size());
        SOIL_free_image_data(img);
    }
}

GLuint Texture2DLoader::_upload()
{
    GLuint texture = 0;
    if (width > 0 && height > 0 && !data.empty())
    {
        glGenTextures(1, &texture);
        if (texture > 0)
        {
            glBindTexture(GL_TEXTURE_2D, texture); gl_bugcheck();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); gl_bugcheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); gl_bugcheck();

            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data()); gl_bugcheck();
            glGenerateMipmap(GL_TEXTURE_2D); gl_bugcheck();

            //glBindTexture(GL_TEXTURE_2D, 0); gl_bugcheck();

            //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            //gl_bugcheck();

        }
    }
    return texture;
}
