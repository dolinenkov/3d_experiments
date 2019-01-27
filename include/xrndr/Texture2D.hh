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

    static void bind(int unit, const Texture2D & texture);
    static void unbind(int unit);

public:
    GLuint name;
};

}
