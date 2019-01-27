#include <xrndr/Texture2D.hh>

namespace xrndr
{

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
    bind(unit, *this);
}

void Texture2D::bind(int unit, const Texture2D & texture)
{
    glActiveTexture(GL_TEXTURE0 + unit); gl_bugcheck();
    glBindTexture(GL_TEXTURE_2D, texture.name); gl_bugcheck();
}

void Texture2D::unbind(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit); gl_bugcheck();
    glBindTexture(GL_TEXTURE_2D, 0); gl_bugcheck();
}


}