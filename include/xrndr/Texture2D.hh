#pragma once
#include <xrndr/config.hh>


namespace xrndr
{

class Texture2D
{
public:
    explicit Texture2D(GLuint name);

    ~Texture2D();

    void use(int unit);

public:
    GLuint name;
};

}
