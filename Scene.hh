#pragma once
#include "Camera.hh"
#include "Program.hh"
#include "Texture2D.hh"
#include "Model.hh"
#include "VertexFormat.hh"
#include "MatrixStack.hh"
#include "Projection.hh"


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
    shared_ptr<Texture2D>       textureSpecular;

    shared_ptr<Plane>           plane;

    shared_ptr<Model>           sphereModel;

    shared_ptr<Model>           cubeModel;

    shared_ptr<VertexFormat>    format;
};
