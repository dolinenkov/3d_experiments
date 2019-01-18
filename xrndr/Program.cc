#include <xrndr/Program.hh>

namespace xrndr
{

Program::Program(GLuint id) : id(id)
{
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "shader program (%d)\n", id);
    _loadActiveAttributes();
    _loadActiveUniforms();
}

void Program::use()
{
    glUseProgram(id); gl_bugcheck();
}

GLint Program::findAttribute(const char * name)
{
    auto it = activeAttribs.find(name);
    assert(it != activeAttribs.end());
    return it != activeAttribs.end() ? it->second.index : -1;
}

GLint Program::findUniform(const char * name)
{
    auto it = activeUniforms.find(name);
    assert(it != activeUniforms.end());
    return it != activeUniforms.end() ? it->second.index : -1;
}

void Program::setFloat(const char * name, float f)
{
    glUniform1f(findUniform(name), f); gl_bugcheck();
}

void Program::setVec3(const char * name, const vec3 & vec)
{
    glUniform3f(findUniform(name), vec[0], vec[1], vec[2]); gl_bugcheck();
}

void Program::setVec4(const char * name, const vec4 & vec)
{
    glUniform4f(findUniform(name), vec[0], vec[1], vec[2], vec[3]); gl_bugcheck();
}

void Program::setMat3(const char * name, const mat3 & m)
{
    glUniformMatrix3fv(findUniform(name), 1, GL_FALSE, value_ptr(m)); gl_bugcheck();
}

void Program::setMat4(const char * name, const mat4 & mat)
{
    glUniformMatrix4fv(findUniform(name), 1, GL_FALSE, value_ptr(mat)); gl_bugcheck();
}

void Program::setTexture(const char * name, int unit)
{
    glUniform1i(findUniform(name), unit); gl_bugcheck();
}

void Program::_loadActiveAttributes()
{
    if (id > 0)
    {
        GLint count = 0;
        glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count); gl_bugcheck();

        GLint maxLength = 0;
        glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength); gl_bugcheck();

        vector<GLchar> buf(maxLength + 1);
        for (GLint i = 0; i < count; ++i)
        {
            GLsizei len = 0;
            GLsizei size = 0;
            GLenum type = 0;
            glGetActiveAttrib(id, i, maxLength, &len, &size, &type, buf.data()); gl_bugcheck();

            activeAttribs.insert({ string(buf.data(), len), {i, type, size} });
            SDL_Log("%10s (%40s:%2d) : %10s, %2d\n", "attribute", buf.data(), i, _getAttributeNameByType(type), size);
        }
    }
}

void Program::_loadActiveUniforms()
{
    if (id > 0)
    {
        GLint count = 0;
        glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count); gl_bugcheck();

        GLint maxLength = 0;
        glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength); gl_bugcheck();

        vector<GLchar> buf(maxLength + 1);
        for (GLint i = 0; i < count; ++i)
        {
            GLsizei len = 0;
            GLsizei size = 0;
            GLenum type = 0;
            glGetActiveUniform(id, i, maxLength, &len, &size, &type, buf.data()); gl_bugcheck();

            activeUniforms.insert({ string(buf.data(), len), {i, type, size} });
            SDL_Log("%10s (%40s:%2d) : %10s, %2d\n", "uniform", buf.data(), i, _getUniformNameByType(type), size);
        }
    }
}

#define _CASE_TO_STR(x) case x: return #x;

const char * Program::_getAttributeNameByType(GLenum type) const
{
    switch (type)
    {
        _CASE_TO_STR(GL_FLOAT);
        _CASE_TO_STR(GL_FLOAT_VEC2);
        _CASE_TO_STR(GL_FLOAT_VEC3);
        _CASE_TO_STR(GL_FLOAT_VEC4);
        _CASE_TO_STR(GL_FLOAT_MAT2);
        _CASE_TO_STR(GL_FLOAT_MAT3);
        _CASE_TO_STR(GL_FLOAT_MAT4);
        _CASE_TO_STR(GL_FLOAT_MAT2x3);
        _CASE_TO_STR(GL_FLOAT_MAT2x4);
        _CASE_TO_STR(GL_FLOAT_MAT3x2);
        _CASE_TO_STR(GL_FLOAT_MAT3x4);
        _CASE_TO_STR(GL_FLOAT_MAT4x2);
        _CASE_TO_STR(GL_FLOAT_MAT4x3);
        _CASE_TO_STR(GL_INT);
        _CASE_TO_STR(GL_INT_VEC2);
        _CASE_TO_STR(GL_INT_VEC3);
        _CASE_TO_STR(GL_INT_VEC4);
        _CASE_TO_STR(GL_UNSIGNED_INT);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC2);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC3);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC4);
        _CASE_TO_STR(GL_DOUBLE);
        _CASE_TO_STR(GL_DOUBLE_VEC2);
        _CASE_TO_STR(GL_DOUBLE_VEC3);
        _CASE_TO_STR(GL_DOUBLE_VEC4);
        _CASE_TO_STR(GL_DOUBLE_MAT2);
        _CASE_TO_STR(GL_DOUBLE_MAT3);
        _CASE_TO_STR(GL_DOUBLE_MAT4);
        _CASE_TO_STR(GL_DOUBLE_MAT2x3);
        _CASE_TO_STR(GL_DOUBLE_MAT2x4);
        _CASE_TO_STR(GL_DOUBLE_MAT3x2);
        _CASE_TO_STR(GL_DOUBLE_MAT3x4);
        _CASE_TO_STR(GL_DOUBLE_MAT4x2);
        _CASE_TO_STR(GL_DOUBLE_MAT4x3);
    default:
    break;
    }
    return "";
}

const char * Program::_getUniformNameByType(GLenum type) const
{
    switch (type)
    {
        _CASE_TO_STR(GL_FLOAT);
        _CASE_TO_STR(GL_FLOAT_VEC2);
        _CASE_TO_STR(GL_FLOAT_VEC3);
        _CASE_TO_STR(GL_FLOAT_VEC4);
        _CASE_TO_STR(GL_DOUBLE);
        _CASE_TO_STR(GL_DOUBLE_VEC2);
        _CASE_TO_STR(GL_DOUBLE_VEC3);
        _CASE_TO_STR(GL_DOUBLE_VEC4);
        _CASE_TO_STR(GL_INT);
        _CASE_TO_STR(GL_INT_VEC2);
        _CASE_TO_STR(GL_INT_VEC3);
        _CASE_TO_STR(GL_INT_VEC4);
        _CASE_TO_STR(GL_UNSIGNED_INT);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC2);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC3);
        _CASE_TO_STR(GL_UNSIGNED_INT_VEC4);
        _CASE_TO_STR(GL_BOOL);
        _CASE_TO_STR(GL_BOOL_VEC2);
        _CASE_TO_STR(GL_BOOL_VEC3);
        _CASE_TO_STR(GL_BOOL_VEC4);
        _CASE_TO_STR(GL_FLOAT_MAT2);
        _CASE_TO_STR(GL_FLOAT_MAT3);
        _CASE_TO_STR(GL_FLOAT_MAT4);
        _CASE_TO_STR(GL_FLOAT_MAT2x3);
        _CASE_TO_STR(GL_FLOAT_MAT2x4);
        _CASE_TO_STR(GL_FLOAT_MAT3x2);
        _CASE_TO_STR(GL_FLOAT_MAT3x4);
        _CASE_TO_STR(GL_FLOAT_MAT4x2);
        _CASE_TO_STR(GL_FLOAT_MAT4x3);
        _CASE_TO_STR(GL_DOUBLE_MAT2);
        _CASE_TO_STR(GL_DOUBLE_MAT3);
        _CASE_TO_STR(GL_DOUBLE_MAT4);
        _CASE_TO_STR(GL_DOUBLE_MAT2x3);
        _CASE_TO_STR(GL_DOUBLE_MAT2x4);
        _CASE_TO_STR(GL_DOUBLE_MAT3x2);
        _CASE_TO_STR(GL_DOUBLE_MAT3x4);
        _CASE_TO_STR(GL_DOUBLE_MAT4x2);
        _CASE_TO_STR(GL_DOUBLE_MAT4x3);
        _CASE_TO_STR(GL_SAMPLER_1D);
        _CASE_TO_STR(GL_SAMPLER_2D);
        _CASE_TO_STR(GL_SAMPLER_3D);
        _CASE_TO_STR(GL_SAMPLER_CUBE);
        _CASE_TO_STR(GL_SAMPLER_1D_SHADOW);
        _CASE_TO_STR(GL_SAMPLER_2D_SHADOW);
        _CASE_TO_STR(GL_SAMPLER_1D_ARRAY);
        _CASE_TO_STR(GL_SAMPLER_2D_ARRAY);
        _CASE_TO_STR(GL_SAMPLER_1D_ARRAY_SHADOW);
        _CASE_TO_STR(GL_SAMPLER_2D_ARRAY_SHADOW);
        _CASE_TO_STR(GL_SAMPLER_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_SAMPLER_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_SAMPLER_CUBE_SHADOW);
        _CASE_TO_STR(GL_SAMPLER_BUFFER);
        _CASE_TO_STR(GL_SAMPLER_2D_RECT);
        _CASE_TO_STR(GL_SAMPLER_2D_RECT_SHADOW);
        _CASE_TO_STR(GL_INT_SAMPLER_1D);
        _CASE_TO_STR(GL_INT_SAMPLER_2D);
        _CASE_TO_STR(GL_INT_SAMPLER_3D);
        _CASE_TO_STR(GL_INT_SAMPLER_CUBE);
        _CASE_TO_STR(GL_INT_SAMPLER_1D_ARRAY);
        _CASE_TO_STR(GL_INT_SAMPLER_2D_ARRAY);
        _CASE_TO_STR(GL_INT_SAMPLER_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_INT_SAMPLER_BUFFER);
        _CASE_TO_STR(GL_INT_SAMPLER_2D_RECT);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_1D);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_2D);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_3D);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_CUBE);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_BUFFER);
        _CASE_TO_STR(GL_UNSIGNED_INT_SAMPLER_2D_RECT);
        _CASE_TO_STR(GL_IMAGE_1D);
        _CASE_TO_STR(GL_IMAGE_2D);
        _CASE_TO_STR(GL_IMAGE_3D);
        _CASE_TO_STR(GL_IMAGE_2D_RECT);
        _CASE_TO_STR(GL_IMAGE_CUBE);
        _CASE_TO_STR(GL_IMAGE_BUFFER);
        _CASE_TO_STR(GL_IMAGE_1D_ARRAY);
        _CASE_TO_STR(GL_IMAGE_2D_ARRAY);
        _CASE_TO_STR(GL_IMAGE_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_IMAGE_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_INT_IMAGE_1D);
        _CASE_TO_STR(GL_INT_IMAGE_2D);
        _CASE_TO_STR(GL_INT_IMAGE_3D);
        _CASE_TO_STR(GL_INT_IMAGE_2D_RECT);
        _CASE_TO_STR(GL_INT_IMAGE_CUBE);
        _CASE_TO_STR(GL_INT_IMAGE_BUFFER);
        _CASE_TO_STR(GL_INT_IMAGE_1D_ARRAY);
        _CASE_TO_STR(GL_INT_IMAGE_2D_ARRAY);
        _CASE_TO_STR(GL_INT_IMAGE_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_1D);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_2D);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_3D);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_2D_RECT);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_CUBE);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_BUFFER);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_1D_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_2D_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE);
        _CASE_TO_STR(GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY);
        _CASE_TO_STR(GL_UNSIGNED_INT_ATOMIC_COUNTER);
    default:
    break;
    }
    return "";
}

#undef _CASE_TO_STR

ProgramBuilder::ProgramBuilder()
{
}

ProgramBuilder::~ProgramBuilder()
{
    _cleanup();
}

void ProgramBuilder::addFragmentShaderFromFile(const char * filename)
{
    vector<char> data;
    if (_loadFile(filename, data))
    {
        auto shader = _buildShader(GL_FRAGMENT_SHADER, data);
        if (shader > 0)
            shaders.push_back(shader);
    }
}

void ProgramBuilder::addVertexShaderFromFile(const char * filename)
{
    vector<char> data;
    if (_loadFile(filename, data))
    {
        auto shader = _buildShader(GL_VERTEX_SHADER, data);
        if (shader > 0)
            shaders.push_back(shader);
    }
}

shared_ptr<Program> ProgramBuilder::build()
{
    return make_shared<Program>(_linkProgram());
}

bool ProgramBuilder::_loadFile(const char * filename, vector<char> & data)
{
    bool read = false;
    auto file = SDL_RWFromFile(filename, "rb");
    assert(file != nullptr);
    if (file)
    {
        data.resize(SDL_RWsize(file));
        SDL_RWread(file, data.data(), data.size(), 1);
        SDL_RWclose(file);
        read = true;
    }
    return read;
}

GLuint ProgramBuilder::_buildShader(GLenum type, const vector<char> & data)
{
    GLuint shader = glCreateShader(type);
    if (shader > 0)
    {
        const GLchar * d[] = { data.data() };
        const GLint s[] = { data.size() };
        glShaderSource(shader, 1, d, s);

        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            vector<char> log(length + 1);
            glGetShaderInfoLog(shader, length, nullptr, log.data());
            SDL_Log("glsl compiler: %s\n", log.data());

            glDeleteShader(shader);
            shader = 0;
        }
    }

    return shader;
}

GLuint ProgramBuilder::_linkProgram()
{
    GLuint program = 0;
    if (!shaders.empty())
    {
        program = glCreateProgram();
        if (program > 0)
        {
            for (GLuint shader : shaders)
                glAttachShader(program, shader);

            GLint status = GL_FALSE;
            glLinkProgram(program);
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (status == GL_TRUE)
            {
                status = GL_FALSE;
                glValidateProgram(program);
                glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
            }

            if (status != GL_TRUE)
            {
                GLint length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

                vector<char> log(length + 1);
                glGetProgramInfoLog(program, length, nullptr, log.data());
                SDL_Log("glsl linker: %s\n", log.data());
            }

            for (GLuint shader : shaders)
                glDetachShader(program, shader);

            if (status != GL_TRUE)
            {
                glDeleteProgram(program);
                program = 0;
            }
        }
        _cleanup();
    }
    return program;
}

void ProgramBuilder::_cleanup()
{
    for (GLuint shader : shaders)
        glDeleteShader(shader);
    shaders.clear();
}

}
