#include "config.hh"
#include "Scene.hh"

struct Keyboard
{
    bitset<SDL_NUM_SCANCODES> states;

    bitset<SDL_NUM_SCANCODES> released;
};

int main(int, char *[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    if (auto window = SDL_CreateWindow("textured cube (OpenGL 3.3)", 100, 100, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
    {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        if (auto context = SDL_GL_CreateContext(window))
        {
            const auto glewError = glewInit();
            if (glewError != GLEW_NO_ERROR)
            {
                SDL_Log("glew init: %s\n", glewGetErrorString(glewError));
            }
            else
            if (auto scene = std::make_unique<Scene>())
            {
                if (GLEW_KHR_debug)
                {
                    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
                    {
                        SDL_Log("gl debug: %s\n", message);
                    }, nullptr);
                }

                Keyboard keyboard;

                SDL_Event event;

                for (bool running = true; running;)
                {
                    float rx = 0.0f;
                    float ry = 0.0f;

                    int wx = 0;
                    int wy = 0;
                    SDL_GetWindowSize(window, &wx, &wy);

                    SDL_PumpEvents();
                    keyboard.released.reset();

                    while (SDL_PollEvent(&event))
                    {
                        if (event.type == SDL_QUIT)
                        {
                            running = false;
                            break;
                        }
                        else if (event.type == SDL_KEYDOWN)
                        {
                            keyboard.states[event.key.keysym.scancode] = true;
                        }
                        else if (event.type == SDL_KEYUP)
                        {
                            if (keyboard.states[event.key.keysym.scancode])
                                keyboard.released[event.key.keysym.scancode] = true;
                            keyboard.states[event.key.keysym.scancode] = false;
                        }
                        else if (event.type == SDL_MOUSEMOTION && wx > 0 && wy > 0)
                        {
                            rx += event.motion.xrel / static_cast<float>(wx);
                            ry += event.motion.yrel / static_cast<float>(wy);
                        }
                    }

                    if (keyboard.states[SDL_SCANCODE_W])
                        scene->moveZ(true);
                    if (keyboard.states[SDL_SCANCODE_S])
                        scene->moveZ(false);

                    if (keyboard.states[SDL_SCANCODE_A])
                        scene->moveX(false);
                    if (keyboard.states[SDL_SCANCODE_D])
                        scene->moveX(true);

                    if (keyboard.states[SDL_SCANCODE_Q])
                        scene->moveY(true);
                    if (keyboard.states[SDL_SCANCODE_E])
                        scene->moveY(false);

                    if (keyboard.released[SDL_SCANCODE_F1])
                        scene->toggleMode();

                    scene->rotateX(rx);
                    scene->rotateZ(ry);

                    scene->draw();

                    SDL_GL_SwapWindow(window);
                }
            }

            SDL_GL_DeleteContext(context);
        }
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return 0;
}