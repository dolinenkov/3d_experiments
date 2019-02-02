#include <xrndr/Settings.hh>
#include <xrndr/json.hh>

namespace xrndr
{

Settings::Settings()
{
    defaults();
}

void Settings::defaults()
{
    vsync = true;
    gamma = 2.2f;
}

void Settings::load()
{
    defaults();

    vector<char> d;

    if (auto f = SDL_RWFromFile("settings.json", "r"))
    {
        auto s = SDL_RWsize(f);
        d.resize(s);
        SDL_RWread(f, d.data(), d.size(), 1);
        SDL_RWclose(f);
    }

    const json::Object settingsJson(d.data(), d.size());
    if (!settingsJson.empty())
    {
        if (settingsJson.has("gamma"))
        {
            gamma = settingsJson.getFloat("gamma");
        }

        if (settingsJson.has("vsync"))
        {
            vsync = settingsJson.getBool("vsync");
        }
    }

}

void Settings::save()
{
}

}
