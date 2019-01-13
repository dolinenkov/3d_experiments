#pragma once
#include "config.hh"


class Program
{
public:
    struct Attachment
    {
        GLint   index;
        GLenum  type;
        GLint   size;
    };

    explicit Program(GLuint id);

    void use();

    GLint findAttribute(const char * name);

    GLint findUniform(const char * name);

    void setFloat(const char * name, float f);

    void setVec3(const char * name, const vec3 & vec);

    void setVec4(const char * name, const vec4 & vec);

    void setMat3(const char * name, const mat3 & m);

    void setMat4(const char * name, const mat4 & mat);

    void setTexture(const char * name, int unit);

    void _loadActiveAttributes();
    void _loadActiveUniforms();

    const char * _getAttributeNameByType(GLenum type) const;
    const char * _getUniformNameByType(GLenum type) const;

    GLuint                  id;
    map<string, Attachment> activeAttribs;
    map<string, Attachment> activeUniforms;
};


class ProgramBuilder
{
public:
    ProgramBuilder();

    ~ProgramBuilder();

    void addFragmentShaderFromFile(const char * filename);

    void addVertexShaderFromFile(const char * filename);

    shared_ptr<Program> build();

    bool _loadFile(const char * filename, vector<char> & data);

    GLuint _buildShader(GLenum type, const vector<char> & data);

    GLuint _linkProgram();

    void _cleanup();

private:
    vector<GLuint> shaders;
};
