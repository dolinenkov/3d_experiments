#pragma once
#include <xrndr/config.hh>

namespace xrndr
{

class Settings
{
public:
    Settings();

    void defaults();

    void load();

    void save();

    bool vsync;
    float gamma;
};

}