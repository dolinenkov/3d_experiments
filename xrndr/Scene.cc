#include <xrndr/Scene.hh>

namespace xrndr
{

bool DepthTestEnabled = true;
bool FaceCullingEnabled = false;


Scene::Scene()
    : mode(true)
{
    ProgramBuilder builder;
    Texture2DLoader loader;

    // programs

    builder.addVertexShaderFromFile("resources/shaders/first_pass.vert");
    builder.addFragmentShaderFromFile("resources/shaders/first_pass.frag");
    firstPassProgram = builder.build();

    builder.addVertexShaderFromFile("resources/shaders/nontextured_geometry.vert");
    builder.addFragmentShaderFromFile("resources/shaders/nontextured_geometry.frag");
    nontexturedGeometryProgram = builder.build();

    // textures

    loader.setFilename("resources/textures/container2.png");
    textureDiffuse = loader.load();

    //

    VerticeFormat geometryPassVerticeFormat;
    geometryPassVerticeFormat.position = firstPassProgram->findAttribute("a_Position");
    geometryPassVerticeFormat.texture = firstPassProgram->findAttribute("a_Texture");

    VerticeFormat nontexturedGeometryVerticeFormat;
    nontexturedGeometryVerticeFormat.position = nontexturedGeometryProgram->findAttribute("a_Position");

    //

    light = make_shared<Light>();
    light->color = vec3(1.0f, 1.0f, 1.0f);
    light->position = vec3(0.0f, 2.0f, 1.0f);

    //

    sphereModel = make_shared<Model>();
    sphereModel->loadFromFile("resources/models/sphere.obj", geometryPassVerticeFormat);
    sphereModel->setPosition(vec3(-1.0f, 0.0f, 5.0f));
    sphereModel->setScale(vec3(0.05f, 0.05f, 0.05f));

    cubeModel = make_shared<Model>();
    cubeModel->loadFromFile("resources/models/cube.obj", geometryPassVerticeFormat);
    cubeModel->setPosition(vec3(1.0f, 0.0f, 5.0f));

    lightModel = make_shared<Model>();
    lightModel->loadFromFile("resources/models/cube.obj", nontexturedGeometryVerticeFormat);
    lightModel->setPosition(light->position);

    //

    camera = make_shared<Camera>();
    camera->setPosition(vec3(0.0f, 0.0f, 0.0f));
    camera->setFrontVector(vec3(0.0f, 0.0f, 1.0f)); // look along +z axis, +x is forwarded to the left, +y is forwarded upwards

    projection = make_shared<Projection>();

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    (DepthTestEnabled ? glEnable : glDisable)(GL_DEPTH_TEST);
    (FaceCullingEnabled ? glEnable : glDisable)(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    _updateMode();

}

Scene::~Scene()
{
}

void Scene::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_bugcheck();

    _doGeometryPass();

    _doDebugPass();
}

void Scene::toggleMode()
{
    mode = !mode;
    _updateMode();
}

shared_ptr<Camera> Scene::getCamera()
{
    return camera;
}

void Scene::updateViewport(int width, int height)
{
    if (width > 0 && height > 0)
    {
        projection->makePerspective(60.0, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
        glViewport(0, 0, width, height);
    }
}

void Scene::_doGeometryPass()
{
    auto transformProjection = matrixStackSet.transformProjection(projection->getProjectionMatrix());
    auto transformView = matrixStackSet.transformView(camera->getViewMatrix());
    auto transformModel = matrixStackSet.transformModel(mat4());

    firstPassProgram->use();

    if (cubeModel)
        _drawModel(*cubeModel);

    if (sphereModel)
        _drawModel(*sphereModel);
}

void Scene::_doDebugPass()
{
    auto transformProjection = matrixStackSet.transformProjection(projection->getProjectionMatrix());
    auto transformView = matrixStackSet.transformView(camera->getViewMatrix());
    auto transformModel = matrixStackSet.transformModel(mat4());

    nontexturedGeometryProgram->use();

    if (lightModel && light)
        _drawLight(*light, *lightModel);
}

void Scene::_drawModel(Model & model)
{
    auto transformModel = matrixStackSet.transformModel(model.getModelMatrix(), true);

    const auto & matrixGroup = matrixStackSet.getMatrixGroup();

    firstPassProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjectionMatrix);

    const GLint unit = 0;
    textureDiffuse->use(unit);
    firstPassProgram->setTexture("u_DiffuseTexture", unit);

    model.draw();
}

void Scene::_drawLight(Light & light, Model & model)
{
    auto transformModel = matrixStackSet.transformModel(model.getModelMatrix(), true);

    const auto & matrixGroup = matrixStackSet.getMatrixGroup();

    nontexturedGeometryProgram->setVec3("u_Color", light.color);
    nontexturedGeometryProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjectionMatrix);

    model.draw();
}

void Scene::_updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}


}
