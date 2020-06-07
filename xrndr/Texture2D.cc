#include <xrndr/Texture2D.hh>

namespace xrndr
{

Texture2D::~Texture2D()
{
    if (name > 0)
    {
        glDeleteTextures(1, &name); gl_bugcheck();
    }
}

bool Texture2D::is_ready() const
{
    return _state == State::Ready;
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

std::shared_ptr<Texture2D> Texture2D::Loader::load(const Description& description) const
{
    return load(description, ExecutionContext::current());
}

std::shared_ptr<Texture2D> Texture2D::Loader::load(const Description& description, ExecutionContext& context) const
{
    auto texture = std::make_shared<Texture2D>();
    texture->_state = State::Await;

    //TODO: encoded/decoded images are most likely copied here in an inefficient way. should be investigated and fixed
    ThreadPool::get().post([description, texture, &context]()
    {
        do
        {
            uint32_t width, height, channels;
            width = height = channels = 0;
            std::vector<char> encoded, decoded;

            if (!read_file(fmt::format("resources/textures/{}", description.path), encoded)) break;
            if (!decode_image(encoded, width, height, channels, decoded)) break;

            context.post([texture, width, height, channels, srgb = description.srgb, decoded = std::move(decoded)]()
            {
                GLuint handle = 0;
                if (upload_texture(decoded, width, height, channels, srgb, handle))
                {
                    texture->name = handle;
                    texture->_state = State::Ready;
                }
                else
                {
                    texture->_state = State::Error;
                }
            });

            return;
        } while (false);
        texture->_state = State::Error;
    });

    return texture;
}

bool Texture2D::Loader::read_file(Path path, std::vector<char>& data)
{
    do
    {
        if (path.empty()) break;
        auto file = SDL_RWFromFile(path.c_str(), "rb");
        if (!file) break;
        data.resize(SDL_RWsize(file));
        SDL_RWread(file, data.data(), data.size(), 1);
        SDL_RWclose(file);
        return true;
    } while (false);
    return false;
}

bool Texture2D::Loader::decode_image(const std::vector<char>& file, uint32_t& width, uint32_t& height, uint32_t& channels, std::vector<char>& decoded)
{
    do
    {
        int w, h, c;
        w = h = c = 0;
        auto img = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(file.data()), static_cast<int>(file.size()), &w, &h, &c, 4);
        if (!img) break;

        width = w;
        height = h;
        channels = c;

        decoded.resize(width * height * channels);
        memcpy(decoded.data(), img, decoded.size());
        stbi_image_free(img);
        return true;
    } while (false);
    return false;
}

bool Texture2D::Loader::upload_texture(const std::vector<char>& image, uint32_t width, uint32_t height, uint32_t channels, bool srgb, GLuint& texture)
{
    GLenum in_format;
    switch (channels)
    {
    case 3:
        in_format = GL_RGB;
        break;

    case 4:
        in_format = GL_RGBA;
        break;

    default:
        assert(0);
        return false;
    }

    GLuint handle = 0;
    do
    {
        glGenTextures(1, &handle);
        if (glGetError()) break;

        glBindTexture(GL_TEXTURE_2D, handle);
        if (glGetError()) break;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        if (glGetError()) break;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        if (glGetError()) break;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        if (glGetError()) break;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (glGetError()) break;

        glTexImage2D(GL_TEXTURE_2D, 0, srgb ? GL_SRGB : GL_RGB, width, height, 0, in_format, GL_UNSIGNED_BYTE, image.data());
        if (glGetError()) break;

        glGenerateMipmap(GL_TEXTURE_2D);
        if (glGetError()) break;

        texture = handle;
        return true;
    } while (false);

    if (handle > 0)
    {
        glDeleteTextures(1, &handle);
    }

    return false;
}

Texture2D::Cache& Texture2D::Cache::instance()
{
    static Texture2D::Cache cache;
    return cache;
}

}
