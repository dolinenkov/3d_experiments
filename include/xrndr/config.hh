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


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace xrndr
{

using ::glm::vec2;
using ::glm::vec3;
using ::glm::vec4;
using ::glm::mat3;
using ::glm::mat4;
using ::glm::ortho;
using ::glm::perspective;
using ::glm::lookAt;
using ::glm::translate;
using ::glm::rotate;
using ::glm::scale;
using ::glm::value_ptr;
using ::glm::radians;
using ::glm::inverse;
using ::glm::transpose;
using ::glm::length;
using ::glm::half_pi;
using ::glm::clamp;
using ::glm::normalize;
using ::glm::yawPitchRoll;

}


#include <fmt/format.h>

namespace xrndr
{

using ::fmt::format;

}


#include <SOIL/SOIL.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <vector>
#include <list>
#include <string>
#include <map>
#include <memory>
#include <bitset>
#include <functional>

namespace xrndr
{

using ::std::vector;
using ::std::list;
using ::std::string;
using ::std::map;
using ::std::unique_ptr;
using ::std::make_unique;
using ::std::shared_ptr;
using ::std::make_shared;
using ::std::addressof;
using ::std::bitset;
using ::std::move;
using ::std::function;

using Path = ::std::string; // this is going to be a separate class, but now I have no time for this

}


// TODO: move this macro-shit somewhere else


#ifdef _DEBUG
#define gl_bugcheck() ::xrndr::_check_gl_bugs(__FILE__, __LINE__)
#else
#define gl_bugcheck()
#endif


namespace xrndr
{

inline void _check_gl_bugs(const char * file, int line)
{
    const auto err = glGetError();
    if (err != GL_NO_ERROR)
    {
        SDL_Log("gl bugcheck: at %s, %d : 0x%04x : %s\n", file, line, err, glewGetErrorString(err));
    #ifdef _MSC_VER
        __debugbreak();
    #endif
    }
}

}
