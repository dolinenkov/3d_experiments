#include <xrndr/Scene.hh>
#include <xrndr/Texture2DLoader.hh>


namespace xrndr
{

bool DepthTestEnabled = true;
bool FaceCullingEnabled = false;


Scene::Scene()
    : mode(true)
{
    ProgramBuilder builder;

    Texture2DLoader loader;
    loader.setTextureCache(&_textureCache);

    // programs

    builder.addVertexShaderFromFile("first_pass.vert");
    builder.addFragmentShaderFromFile("first_pass.frag");
    firstPassProgram = builder.build();

    builder.addVertexShaderFromFile("nontextured_geometry.vert");
    builder.addFragmentShaderFromFile("nontextured_geometry.frag");
    nontexturedGeometryProgram = builder.build();

    // textures

    loader.setFilename("container2.png");
    textureDiffuse = loader.load();

    loader.setFilename("container2_specular.png");
    textureSpecular = loader.load();

    //

    VerticeFormat geometryPassVerticeFormat;
    geometryPassVerticeFormat.position = firstPassProgram->findAttribute("a_Position");
    geometryPassVerticeFormat.texture = firstPassProgram->findAttribute("a_Texture");
    geometryPassVerticeFormat.normal = firstPassProgram->findAttribute("a_Normal");

    VerticeFormat nontexturedGeometryVerticeFormat;
    nontexturedGeometryVerticeFormat.position = nontexturedGeometryProgram->findAttribute("a_Position");

    //

    light = make_shared<Light>();
    light->position     = vec3(0.0f, 0.0f, 5.0f);
    light->color        = vec3(1.0f, 1.0f, 1.0f);
    light->attenuation  = vec3(0.0f, 0.1f, 0.02f);
    light->intensity    = vec3(0.1f, 0.7f, 1.0f);

    lightPhase = 0.0f;

    //

    sphereModel = make_shared<Model>();
    sphereModel->loadFromFile("cube.obj", geometryPassVerticeFormat);
    sphereModel->setPosition(vec3(-1.0f, 0.0f, 5.0f));
    //sphereModel->setScale(vec3(0.05f, 0.05f, 0.05f));

    cubeModel = make_shared<Model>();
    cubeModel->loadFromFile("cube.obj", geometryPassVerticeFormat);
    cubeModel->setPosition(vec3(1.0f, 0.0f, 5.0f));
    cubeModel->setRotation(vec3(0.1f, 0.2f, 0.3f));

    terrainModel = make_shared<Model>();
    terrainModel->loadFromFile("terrain.obj", geometryPassVerticeFormat);
    terrainModel->setScale(vec3(10.0f, 1.0f, 10.0f));
    terrainModel->setPosition(vec3(0.0f, -2.0f, 0.0f));

    lightModel = make_shared<Model>();
    lightModel->loadFromFile("cube.obj", nontexturedGeometryVerticeFormat);
    lightModel->setScale(vec3(0.2f, 0.2f, 0.2f));

    //

    camera = make_shared<Camera>();
    camera->setPosition(vec3(0.0f, 0.0f, 0.5f));
    camera->setFrontVector(vec3(0.0f, 0.0f, 1.0f)); // look along +z axis, +x is forwarded to the left, +y is forwarded upwards

    projection = make_shared<Projection>();

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    (DepthTestEnabled ? glEnable : glDisable)(GL_DEPTH_TEST);
    (FaceCullingEnabled ? glEnable : glDisable)(GL_CULL_FACE);

    _updateMode();
}

Scene::~Scene()
{
}

void Scene::draw()
{
    _update();

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

void Scene::_update()
{
    lightPhase += 0.01f;
    if (lightPhase > glm::two_pi<float>())
        lightPhase = 0.0f;

    light->position = vec3(2.5f * cos(lightPhase), 2.5f * sin(lightPhase), 2.0f);
}

void Scene::_doGeometryPass()
{
    _matrixStack.pushProjection(projection->getProjectionMatrix());
    _matrixStack.pushView(camera->getViewMatrix());
    _matrixStack.pushModel(mat4(), false);

    firstPassProgram->use();

    if (cubeModel)
        _drawModel(*cubeModel);

    if (sphereModel)
        _drawModel(*sphereModel);

    if (terrainModel)
        _drawModel(*terrainModel);

    _matrixStack.popProjection();
    _matrixStack.popView();
    _matrixStack.popModel();
}

void Scene::_doDebugPass()
{
    _matrixStack.pushProjection(projection->getProjectionMatrix());
    _matrixStack.pushView(camera->getViewMatrix());
    _matrixStack.pushModel(mat4(), false);

    nontexturedGeometryProgram->use();

    if (lightModel && light)
        _drawLight(*light, *lightModel);

    _matrixStack.popProjection();
    _matrixStack.popView();
    _matrixStack.popModel();
}

void Scene::_drawModel(Model & model)
{
    _matrixStack.pushModel(model.getModelMatrix(), true);

    const auto & matrixGroup = _matrixStack.getCache();

    firstPassProgram->setMat4("u_ModelMatrix", matrixGroup.model);
    firstPassProgram->setMat4("u_ViewMatrix", matrixGroup.view);
    firstPassProgram->setMat4("u_ProjectionMatrix", matrixGroup.projection);

    firstPassProgram->setVec3("u_Camera.position", camera->getPosition());

    firstPassProgram->setVec3("u_Light.position", light->position);
    firstPassProgram->setVec3("u_Light.color", light->color);
    firstPassProgram->setVec3("u_Light.attenuation", light->attenuation);
    firstPassProgram->setVec3("u_Light.intensity", light->intensity);

    textureDiffuse->use(0);
    textureSpecular->use(1);

    firstPassProgram->setFloat("u_Material.shininess", 1.0f);
    firstPassProgram->setTexture("u_Material.diffuseTexture", 0);
    firstPassProgram->setTexture("u_Material.specularTexture", 1);

    model.draw();

    _matrixStack.popModel();
}

void Scene::_drawLight(Light & light, Model & model)
{
    model.setPosition(light.position);

    _matrixStack.pushModel(model.getModelMatrix(), true);

    const auto & matrixGroup = _matrixStack.getCache();

    nontexturedGeometryProgram->setVec3("u_Color", light.color);
    nontexturedGeometryProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjection);

    model.draw();

    _matrixStack.popModel();
}

void Scene::_updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}

}
