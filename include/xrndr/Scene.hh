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

    void update(float);

    void draw();

    void toggleMode();

    shared_ptr<Camera> getCamera();

    void updateViewport(int width, int height);

private:
    void _doGeometryPass();

    void _doDebugPass();

    void _drawModel(Model & model);

    void _drawPointLight(PointLight & light);

    void _updateMode();

private:
    bool                        _mode;
    MatrixStack                 _matrixStack;
    TextureCache                _textureCache;

    shared_ptr<Projection>      _projection;
    shared_ptr<Camera>          _camera;

    shared_ptr<Program>         _firstPassProgram;

    float                       _lightPhase;
    vector<PointLight>          _pointLights;
    vector<DirectedLight>       _directedLights;

    vector<Model>               _models;

    shared_ptr<Program>         _nontexturedGeometryProgram;
    Model                       _lightVisualizationModel;
};

}
