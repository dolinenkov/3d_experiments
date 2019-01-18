#pragma once
#include <xrndr/Camera.hh>
#include <xrndr/Program.hh>
#include <xrndr/Projection.hh>
#include <xrndr/Texture2D.hh>
#include <xrndr/Model.hh>
#include <xrndr/VertexFormat.hh>
#include <xrndr/MatrixStack.hh>


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
    void updateMode();

    void updateUniforms();

    void drawModel(Model & model);

private:
    bool                        mode;
    MatrixStackSet              matrixStackSet;

    shared_ptr<Projection>      projection;
    shared_ptr<Camera>          camera;

    shared_ptr<Program>         firstPassProgram;

    shared_ptr<Texture2D>       textureDiffuse;

    shared_ptr<Model>           sphereModel;
    shared_ptr<Model>           cubeModel;
};

}
