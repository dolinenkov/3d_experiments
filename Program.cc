#include "Program.hh"

Program::Program(GLuint id): id(id)
{
}

void Program::use()
{
    glUseProgram(id); gl_bugcheck();
}

GLint Program::findAttribute(const char * name)
{
    GLint loc = -1;
    if (id > 0)
    {
        loc = glGetAttribLocation(id, name); gl_bugcheck();
        //assert(loc != -1);
    }
    return loc;
}

GLint Program::findUniform(const char * name)
{
    GLint loc = -1;
    if (id > 0)
    {
        loc = glGetUniformLocation(id, name); gl_bugcheck();
        assert(loc != -1);
    }
    return loc;
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
