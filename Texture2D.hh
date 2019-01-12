#pragma once
#include "config.hh"

class Texture2D
{
public:
    explicit Texture2D(GLuint name);

    ~Texture2D();

    void use(int unit);

public:
    GLuint name;
};


class Texture2DLoader
{
public:
    Texture2DLoader();

    void setFilename(const char * filename);

    void setIsSRGB(bool isSRGB);

    shared_ptr<Texture2D> load();

    bool _loadFile(const char * filename, vector<char> & data);

    void _decode(const vector<char> & data);

    GLuint _upload();

private:
    vector<char> data;
    uint32_t     width;
    uint32_t     height;
    bool         isSRGB;
};
