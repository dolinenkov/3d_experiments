#include <xrndr/config.hh>
#include <xrndr/Scene.hh>
#include <xrndr/Settings.hh>


struct Keyboard
{
    xrndr::bitset<SDL_NUM_SCANCODES> states;

    xrndr::bitset<SDL_NUM_SCANCODES> released;
};


class FrameStats
{
public:
    FrameStats()
        : _count(0)
        , _cur(0.0f)
        , _min(std::numeric_limits<float>::max())
        , _max(0.0f)
        , _avg(0.0f)
    {
    }

    void update(float value)
    {
        if (_count > 0)
        {
            _cur = value;

            if (value < _min)
                _min = value;

            if (value > _max)
                _max = value;

        }

        const float sum = _avg * _count;

        ++_count;

        _avg = (sum + value) / static_cast<float>(_count);
    }

    float cur() const
    {
        return _cur;
    }

    float min() const
    {
        return _min;
    }

    float max() const
    {
        return _max;
    }

    float avg() const
    {
        return _avg;
    }

private:
    int     _count;
    float   _cur;
    float   _min;
    float   _max;
    float   _avg;
};


class TimeManager
{
public:
    TimeManager()
        : _freq(static_cast<double>(SDL_GetPerformanceFrequency()))
        , _init(static_cast<double>(SDL_GetPerformanceCounter()))
        , _last(0.0)
        , _this(0.0)
        , _diff(0.0)
    {

    }

    void update()
    {
        _last = _this;
        _this = (static_cast<double>(SDL_GetPerformanceCounter()) - _init) / _freq;
        _diff = _this - _last;
    }

    float lastFrameTime() const
    {
        return static_cast<float>(_diff);
    }

private:
    double _freq;
    double _init;
    double _last;
    double _this;
    double _diff;
};


int main(int, char *[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    if (auto window = SDL_CreateWindow("textured cube (OpenGL 3.3)", 100, 100, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE))
    {
        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRelativeMouseMode(SDL_TRUE);

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
            int windowWidth;
            int windowHeight;
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);

            xrndr::Settings settings;
            settings.load();

            SDL_GL_SetSwapInterval(settings.vsync ? 1 : 0);

            const auto glewError = glewInit();
            if (glewError != GLEW_NO_ERROR)
            {
                SDL_Log("glew init: %s\n", glewGetErrorString(glewError));
            }
            else
            if (auto scene = xrndr::make_unique<xrndr::Scene>(settings))
            {
                glEnable(GL_DEBUG_OUTPUT);

                if (GLEW_KHR_debug)
                {
                    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam)
                    {
                        SDL_Log("gl debug: %s\n", message);
                    }, nullptr);
                }

                scene->updateViewport(windowWidth, windowHeight);

                Keyboard keyboard;

                SDL_Event event;

                TimeManager timeManager;
                FrameStats frameStats;

                for (bool running = true; running;)
                {
                    timeManager.update();
                    frameStats.update(timeManager.lastFrameTime());

                    const float input_corrector = 1.0f / (60.0f * frameStats.cur());

                    float rx = 0.0f;
                    float ry = 0.0f;

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
                        else if (event.type == SDL_MOUSEMOTION)
                        {
                            rx += event.motion.xrel;
                            ry += event.motion.yrel;
                            SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);
                        }
                        else if (event.type == SDL_WINDOWEVENT)
                        {
                            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                            scene->updateViewport(windowWidth, windowHeight);
                        }
                    }

                    auto camera = scene->getCamera();

                    if (keyboard.states[SDL_SCANCODE_W])
                        camera->moveForward(0.1f / input_corrector);

                    if (keyboard.states[SDL_SCANCODE_S])
                        camera->moveForward(-0.1f / input_corrector);

                    if (keyboard.states[SDL_SCANCODE_A])
                        camera->moveRight(-0.1f / input_corrector);

                    if (keyboard.states[SDL_SCANCODE_D])
                        camera->moveRight(0.1f / input_corrector);

                    if (keyboard.states[SDL_SCANCODE_Q])
                        camera->moveUp(-0.1f / input_corrector);

                    if (keyboard.states[SDL_SCANCODE_E])
                        camera->moveUp(0.1f / input_corrector);

                    if (keyboard.released[SDL_SCANCODE_F1])
                        scene->toggleMode();

                    if (keyboard.released[SDL_SCANCODE_ESCAPE])
                    {
                        SDL_Event quitEvent = {};
                        event.type = SDL_QUIT;
                        SDL_PushEvent(&event);
                    }

                    if (rx != 0.0f)
                        camera->turnRight(rx / static_cast<float>(windowWidth));

                    if (ry != 0.0f)
                        camera->turnUp(ry * -1.0f / static_cast<float>(windowHeight));

                    scene->update(timeManager.lastFrameTime());

                    scene->draw();

                    SDL_GL_SwapWindow(window);

                    {
                        static int i = 0;
                        if (++i > 1000)
                        {
                            i = 0;
                            SDL_LogInfo(
                                SDL_LOG_CATEGORY_APPLICATION,
                                "FPS: avg %5.2f\tcur %5.2f\tmin %5.2f\tmax %5.2f\n",
                                1.0f / frameStats.avg(),
                                1.0f / frameStats.cur(),
                                1.0f / frameStats.max(),
                                1.0f / frameStats.min()
                            );
                        }
                    }
                }
            }

            SDL_GL_DeleteContext(context);
        }

        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_ShowCursor(SDL_ENABLE);

        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return 0;
}
