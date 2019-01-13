#pragma once
#include "Camera.hh"
#include "Program.hh"
#include "Texture2D.hh"
#include "Model.hh"
#include "VertexFormat.hh"
#include "MatrixStack.hh"


class Scene
{
public:
    Scene();
    ~Scene();

    void draw();

    void toggleMode();

    shared_ptr<Camera> getCamera();

private:
    void updateMode();

private:
    bool                        mode;

    MatrixStackSet              matrixStackSet;

    shared_ptr<Camera>          camera;

    shared_ptr<Program>         firstPassProgram;
    shared_ptr<Texture2D>       textureDiffuse;
    shared_ptr<Texture2D>       textureSpecular;
    shared_ptr<Model>           sphereModel;
    shared_ptr<VertexFormat>    format;
};
