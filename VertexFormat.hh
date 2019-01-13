#pragma once
#include "config.hh"
#include "Program.hh"

struct Vertice
{
    float position[3];
    float normal[3];
    float texture[3];
};

// Simple wrapper for OpenGL vertex array object
class VertexFormat
{
public:
    VertexFormat();

    ~VertexFormat();

    void init(shared_ptr<Program> program);

    void activate();

private:
    GLuint arrayObject;
};