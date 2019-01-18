#pragma once
#include "config.hh"
#include "Program.hh"


struct Vertice
{
    float position[3];
    float normal[3];
    float texture[3];
};


struct VerticeFormat
{
    static constexpr GLint NOT_PRESENT = -1;

    GLint position = NOT_PRESENT;
    GLint normal   = NOT_PRESENT;
    GLint texture  = NOT_PRESENT;
};
