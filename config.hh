#pragma once


#include <GL/glew.h>
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#endif


#include <SDL2/SDL.h>
#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#ifdef _DEBUG
#pragma comment(lib, "SDL2maind.lib")
#else
#pragma comment(lib, "SDL2main.lib")
#endif
#endif


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using ::glm::vec2;
using ::glm::vec3;
using ::glm::vec4;
using ::glm::mat3;
using ::glm::mat4;
using ::glm::perspective;
using ::glm::lookAt;
using ::glm::translate;
using ::glm::rotate;
using ::glm::scale;
using ::glm::value_ptr;
using ::glm::radians;
using ::glm::inverse;
using ::glm::transpose;


#include <SOIL/SOIL.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <vector>
#include <string>
#include <stack>
#include <map>
#include <memory>
#include <bitset>

using ::std::vector;
using ::std::string;
using ::std::stack;
using ::std::map;
using ::std::make_unique;
using ::std::shared_ptr;
using ::std::make_shared;
using ::std::addressof;
using ::std::bitset;


// TODO: move this macro-shit somewhere else


#ifdef _DEBUG
#define gl_bugcheck() _check_gl_bugs(__FILE__, __LINE__)
#else
#define gl_bugcheck()
#endif


inline void _check_gl_bugs(const char * file, int line)
{
    const auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        SDL_Log("gl bugcheck: at %s, %d : 0x%04x : %s\n", file, line, err, glewGetErrorString(err));
    }
}
