#pragma once
#include <xrndr/config.hh>
#include <entt/entt.hpp>

namespace xrndr
{

class Texture2D
{
public:
    enum class State
    {
        Empty,
        Await,
        Ready,
        Error,
    };

    struct Description
    {
        Path path;
        bool srgb;
    };

    struct Loader : entt::loader<Loader, Texture2D>
    {
        std::shared_ptr<Texture2D> load(Description) const;

        static bool read_file(Path, std::vector<char>&);

        static bool decode_image(const std::vector<char>& file, uint32_t& width, uint32_t& height, uint32_t& channels, std::vector<char>& decoded);

        static bool upload_texture(const std::vector<char>& image, uint32_t width, uint32_t height, uint32_t channels, bool srgb, GLuint& texture);
    };

    struct Cache : entt::cache<Texture2D>
    {
        static Cache& get();
    };

    Texture2D() = default;
    ~Texture2D();

    bool is_ready() const;

    void use(int unit);

    static void bind(int unit, const Texture2D & texture);
    static void unbind(int unit);

public:
    GLuint name = 0;
    State _state = State::Empty;
};

}
