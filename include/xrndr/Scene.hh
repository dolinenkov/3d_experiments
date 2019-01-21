#pragma once
#include <xrndr/Camera.hh>
#include <xrndr/Program.hh>
#include <xrndr/Projection.hh>
#include <xrndr/Texture2D.hh>
#include <xrndr/Model.hh>
#include <xrndr/VertexFormat.hh>
#include <xrndr/MatrixStack.hh>
#include <xrndr/Light.hh>
#include <xrndr/TextureCache.hh>


namespace xrndr
{

class Scene
{
public:
    Scene();
    ~Scene();

    void draw();

    void toggleMode();

    shared_ptr<Camera> getCamera();

    void updateViewport(int width, int height);

private:
    void _update();

    void _doGeometryPass();

    void _doDebugPass();

    void _drawModel(Model & model);

    void _drawLight(Light & light, Model & model);

    void _updateMode();

private:
    bool                        mode;
    MatrixStack                 _matrixStack;
    TextureCache                _textureCache;

    shared_ptr<Projection>      projection;
    shared_ptr<Camera>          camera;

    shared_ptr<Program>         firstPassProgram;
    shared_ptr<Program>         nontexturedGeometryProgram;

    shared_ptr<Texture2D>       textureDiffuse;
    shared_ptr<Texture2D>       textureSpecular;

    float                       lightPhase;
    shared_ptr<Light>           light;

    shared_ptr<Model>           lightModel;
    shared_ptr<Model>           sphereModel;
    shared_ptr<Model>           cubeModel;
    shared_ptr<Model>           terrainModel;
};

}
