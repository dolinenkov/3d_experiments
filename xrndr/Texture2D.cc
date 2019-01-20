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
    glActiveTexture(GL_TEXTURE0 + unit); gl_bugcheck();
    glBindTexture(GL_TEXTURE_2D, name); gl_bugcheck();
}


}