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

    //

    VerticeFormat geometryPassVerticeFormat;
    geometryPassVerticeFormat.position = firstPassProgram->findAttribute("a_Position");
    geometryPassVerticeFormat.texture = firstPassProgram->findAttribute("a_Texture");
    geometryPassVerticeFormat.normal = firstPassProgram->findAttribute("a_Normal");

    VerticeFormat nontexturedGeometryVerticeFormat;
    nontexturedGeometryVerticeFormat.position = nontexturedGeometryProgram->findAttribute("a_Position");

    //

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 0.0f, 3.0f);
    _pointLights.back().color       = vec3(1.0f, 1.0f, 1.0f);
    _pointLights.back().attenuation = vec3(1.0f, 0.09f, 0.032f);
    _pointLights.back().intensity   = vec3(0.0f, 0.9f, 1.0f);

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 1.0f, -2.0f);
    _pointLights.back().color       = vec3(1.0f, 0.0f, 0.0f);
    _pointLights.back().attenuation = vec3(1.0f, 0.2f, 0.05f);
    _pointLights.back().intensity   = vec3(0.0f, 0.3f, 0.5f);


    _lightPhase = 0.0f;

    //

    _models.emplace_back();
    _models.back().loadFromFile("sphere.obj", geometryPassVerticeFormat, loader);
    _models.back().setPosition(vec3(-1.0f, -1.0f, 5.0f));
    _models.back().setScale(vec3(0.05f, 0.05f, 0.05f));

    _models.emplace_back();
    _models.back().loadFromFile("cube.obj", geometryPassVerticeFormat, loader);
    _models.back().setPosition(vec3(1.0f, -1.5f, 5.0f));

    _models.emplace_back();
    _models.back().loadFromFile("terrain.obj", geometryPassVerticeFormat, loader);
    _models.back().setPosition(vec3(0.0f, -2.0f, 0.0f));
    _models.back().setScale(vec3(10.0f, 1.0f, 10.0f));

    _models.emplace_back();
    _models.back().loadFromFile("torus.obj", geometryPassVerticeFormat, loader);
    _models.back().setPosition(vec3(0.0f, -1.0f, -2.0f));
    _models.back().setScale(vec3(0.5f));


    _lightVisualizationModel.loadFromFile("cube.obj", nontexturedGeometryVerticeFormat, loader);
    _lightVisualizationModel.setScale(vec3(0.2f));

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
    _lightPhase += 0.01f;
    if (_lightPhase > glm::two_pi<float>())
        _lightPhase = 0.0f;

    _pointLights[0].position = vec3(2.5f * cos(_lightPhase), 2.5f * sin(_lightPhase), 2.0f);
}

void Scene::_doGeometryPass()
{
    _matrixStack.pushProjection(projection->getProjectionMatrix());
    _matrixStack.pushView(camera->getViewMatrix());
    _matrixStack.pushModel(mat4(), false);

    firstPassProgram->use();

    firstPassProgram->setVec3("u_Camera.position", camera->getPosition());

    firstPassProgram->setInt("u_PointLightsCount", _pointLights.size());
    for (size_t i = 0; i < _pointLights.size(); ++i)
    {
        firstPassProgram->setVec3(format("u_PointLights[{}].position", i).c_str(), _pointLights[i].position);
        firstPassProgram->setVec3(format("u_PointLights[{}].color", i).c_str(), _pointLights[i].color);
        firstPassProgram->setVec3(format("u_PointLights[{}].attenuation", i).c_str(), _pointLights[i].attenuation);
        firstPassProgram->setVec3(format("u_PointLights[{}].intensity", i).c_str(), _pointLights[i].intensity);
    }

    firstPassProgram->setFloat("u_Material.shininess", 1.0f);
    firstPassProgram->setTexture("u_Material.diffuseTexture", 0);
    firstPassProgram->setTexture("u_Material.specularTexture", 1);

    //

    for (auto & m : _models)
        _drawModel(m);

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

    for (auto & pointLight : _pointLights)
        _drawPointLight(pointLight);

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

    model.draw();

    _matrixStack.popModel();
}

void Scene::_drawPointLight(PointLight & pointLight)
{
    _lightVisualizationModel.setPosition(pointLight.position);

    _matrixStack.pushModel(_lightVisualizationModel.getModelMatrix(), true);

    const auto & matrixGroup = _matrixStack.getCache();

    nontexturedGeometryProgram->setVec3("u_Color", pointLight.color);
    nontexturedGeometryProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjection);

    _lightVisualizationModel.draw();

    _matrixStack.popModel();
}

void Scene::_updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}

}
