#include "VertexFormat.hh"

VertexFormat::VertexFormat(): arrayObject(0)
{
}

VertexFormat::~VertexFormat()
{
    if (arrayObject > 0)
    {
        glDeleteVertexArrays(1, &arrayObject); gl_bugcheck();
    }
}

void VertexFormat::init(shared_ptr<Program> program)
{
    if (program)
    {
        glGenVertexArrays(1, &arrayObject); gl_bugcheck();
        glBindVertexArray(arrayObject); gl_bugcheck();

        const GLint positionLoc = program->findAttribute("a_Position"); gl_bugcheck();
        assert(positionLoc != -1);
        glVertexAttribPointer(positionLoc, sizeof(Vertice::position) / sizeof(Vertice::position[0]), GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<const void *>(offsetof(Vertice, position))); gl_bugcheck();
        glEnableVertexAttribArray(positionLoc); gl_bugcheck();

        const GLint normalLoc = program->findAttribute("a_Normal"); gl_bugcheck();
        //assert(normalLoc != -1);
        if (normalLoc != -1)
        {
            glVertexAttribPointer(normalLoc, sizeof(Vertice::normal) / sizeof(Vertice::normal[0]), GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<const void *>(offsetof(Vertice, normal))); gl_bugcheck();
            glEnableVertexAttribArray(normalLoc); gl_bugcheck();
        }

        const GLint textureLoc = program->findAttribute("a_Texture"); gl_bugcheck();
        assert(textureLoc != -1);
        if (textureLoc != -1)
        {
            glVertexAttribPointer(textureLoc, sizeof(Vertice::texture) / sizeof(Vertice::texture[0]), GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<const void *>(offsetof(Vertice, texture))); gl_bugcheck();
            glEnableVertexAttribArray(textureLoc); gl_bugcheck();
        }
    }
}

void VertexFormat::activate()
{
    glBindVertexArray(arrayObject); gl_bugcheck();
}
