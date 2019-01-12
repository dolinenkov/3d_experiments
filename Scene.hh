#pragma once
#include "Program.hh"
#include "Texture2D.hh"
#include "Model.hh"
#include "VertexFormat.hh"

class Scene
{
public:
    Scene();
    ~Scene();

    void draw();

    void moveX(bool);
    void moveY(bool);
    void moveZ(bool);

    void rotateX(float);
    void rotateZ(float);

    void toggleMode();

private:
    void updateMode();

private:
    bool mode;

    vec3 cameraPosition;
    vec3 cameraDirection;
    float cameraAngles[2];

    stack<mat4> modelStack;
    stack<mat4> viewStack;
    stack<mat4> projectionStack;

    shared_ptr<Program> firstPassProgram;
    shared_ptr<Texture2D> textureDiffuse;
    shared_ptr<Texture2D> textureSpecular;
    shared_ptr<Model> sphereModel;
    shared_ptr<VertexFormat> format;
};
