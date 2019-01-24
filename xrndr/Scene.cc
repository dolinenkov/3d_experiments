#include <xrndr/Scene.hh>
#include <xrndr/Texture2DLoader.hh>


namespace xrndr
{

bool DepthTestEnabled = true;
bool FaceCullingEnabled = false;


Scene::Scene()
    : _mode(true)
{
    ProgramBuilder builder;

    Texture2DLoader loader;
    loader.setTextureCache(&_textureCache);

    // programs

    builder.addVertexShaderFromFile("first_pass.vert");
    builder.addFragmentShaderFromFile("first_pass.frag");
    _firstPassProgram = builder.build();

    builder.addVertexShaderFromFile("nontextured_geometry.vert");
    builder.addFragmentShaderFromFile("nontextured_geometry.frag");
    _nontexturedGeometryProgram = builder.build();

    //

    VerticeFormat geometryPassVerticeFormat;
    geometryPassVerticeFormat.position = _firstPassProgram->findAttribute("a_Position");
    geometryPassVerticeFormat.texture = _firstPassProgram->findAttribute("a_Texture");
    geometryPassVerticeFormat.normal = _firstPassProgram->findAttribute("a_Normal");

    VerticeFormat nontexturedGeometryVerticeFormat;
    nontexturedGeometryVerticeFormat.position = _nontexturedGeometryProgram->findAttribute("a_Position");

    //

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 0.0f, 3.0f);
    _pointLights.back().color       = vec3(1.0f, 1.0f, 1.0f);
    _pointLights.back().attenuation = vec3(1.0f, 0.09f, 0.032f);
    _pointLights.back().intensity   = vec3(0.0f, 0.3f, 0.3f);

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 1.0f, -2.0f);
    _pointLights.back().color       = vec3(1.0f, 0.0f, 0.0f);
    _pointLights.back().attenuation = vec3(1.0f, 0.2f, 0.05f);
    _pointLights.back().intensity   = vec3(0.0f, 0.3f, 0.5f);

    _directedLights.emplace_back();
    _directedLights.back().direction = vec3(-1.0f, 0.0f, -1.0f);
    _directedLights.back().color     = vec3(1.0f, 1.0f, 1.0f);
    _directedLights.back().intensity = vec3(0.0f, 0.3f, 0.5f);

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

    _camera = make_shared<Camera>();
    _camera->setPosition(vec3(0.0f, 0.0f, 0.5f));
    _camera->setFrontVector(vec3(0.0f, 0.0f, 1.0f)); // look along +z axis, +x is forwarded to the left, +y is forwarded upwards

    _projection = make_shared<Projection>();

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    (DepthTestEnabled ? glEnable : glDisable)(GL_DEPTH_TEST);
    (FaceCullingEnabled ? glEnable : glDisable)(GL_CULL_FACE);

    _updateMode();
}

Scene::~Scene()
{
}

void Scene::update(float _s)
{
    _lightPhase += _s;
    if (_lightPhase > glm::two_pi<float>())
        _lightPhase = 0.0f;

    _pointLights[0].position = vec3(2.5f * cos(_lightPhase), 2.5f * sin(_lightPhase), 2.0f);
}

void Scene::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_bugcheck();

    _doGeometryPass();

    _doDebugPass();
}

void Scene::toggleMode()
{
    _mode = !_mode;
    _updateMode();
}

shared_ptr<Camera> Scene::getCamera()
{
    return _camera;
}

void Scene::updateViewport(int width, int height)
{
    if (width > 0 && height > 0)
    {
        _projection->makePerspective(60.0, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
        glViewport(0, 0, width, height);
    }
}

void Scene::_doGeometryPass()
{
    _matrixStack.pushProjection(_projection->getProjectionMatrix());
    _matrixStack.pushView(_camera->getViewMatrix());
    _matrixStack.pushModel(mat4(), false);

    _firstPassProgram->use();

    _firstPassProgram->setVec3("u_Camera.position", _camera->getPosition());

    _firstPassProgram->setInt("u_DirectedLightsCount", _directedLights.size());
    for (size_t i = 0; i < _directedLights.size(); ++i)
    {
        _firstPassProgram->setVec3(format("u_DirectedLights[{}].direction", i).c_str(), _directedLights[i].direction);
        _firstPassProgram->setVec3(format("u_DirectedLights[{}].color", i).c_str(), _directedLights[i].color);
        _firstPassProgram->setVec3(format("u_DirectedLights[{}].intensity", i).c_str(), _directedLights[i].intensity);
    }

    _firstPassProgram->setInt("u_PointLightsCount", _pointLights.size());
    for (size_t i = 0; i < _pointLights.size(); ++i)
    {
        _firstPassProgram->setVec3(format("u_PointLights[{}].position", i).c_str(), _pointLights[i].position);
        _firstPassProgram->setVec3(format("u_PointLights[{}].color", i).c_str(), _pointLights[i].color);
        _firstPassProgram->setVec3(format("u_PointLights[{}].attenuation", i).c_str(), _pointLights[i].attenuation);
        _firstPassProgram->setVec3(format("u_PointLights[{}].intensity", i).c_str(), _pointLights[i].intensity);
    }

    _firstPassProgram->setFloat("u_Material.shininess", 1.0f);
    _firstPassProgram->setTexture("u_Material.diffuseTexture", 0);
    _firstPassProgram->setTexture("u_Material.specularTexture", 1);

    //

    for (auto & m : _models)
        _drawModel(m);

    _matrixStack.popProjection();
    _matrixStack.popView();
    _matrixStack.popModel();
}

void Scene::_doDebugPass()
{
    _matrixStack.pushProjection(_projection->getProjectionMatrix());
    _matrixStack.pushView(_camera->getViewMatrix());
    _matrixStack.pushModel(mat4(), false);

    _nontexturedGeometryProgram->use();

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

    _firstPassProgram->setMat4("u_ModelMatrix",                 matrixGroup.model);
    _firstPassProgram->setMat4("u_ViewMatrix",                  matrixGroup.view);
    _firstPassProgram->setMat4("u_ProjectionMatrix",            matrixGroup.projection);
    _firstPassProgram->setMat4("u_ModelViewMatrix",             matrixGroup.modelView);
    _firstPassProgram->setMat4("u_ViewProjectionMatrix",        matrixGroup.viewProjection);
    _firstPassProgram->setMat4("u_ModelViewProjectionMatrix",   matrixGroup.modelViewProjection);
    _firstPassProgram->setMat3("u_NormalMatrix",                matrixGroup.normal);

    model.draw();

    _matrixStack.popModel();
}

void Scene::_drawPointLight(PointLight & pointLight)
{
    _lightVisualizationModel.setPosition(pointLight.position);

    _matrixStack.pushModel(_lightVisualizationModel.getModelMatrix(), true);

    const auto & matrixGroup = _matrixStack.getCache();

    _nontexturedGeometryProgram->setVec3("u_Color", pointLight.color);

    _nontexturedGeometryProgram->setMat4("u_ModelMatrix",               matrixGroup.model);
    _nontexturedGeometryProgram->setMat4("u_ViewMatrix",                matrixGroup.view);
    _nontexturedGeometryProgram->setMat4("u_ProjectionMatrix",          matrixGroup.projection);
    _nontexturedGeometryProgram->setMat4("u_ModelViewMatrix",           matrixGroup.modelView);
    _nontexturedGeometryProgram->setMat4("u_ViewProjectionMatrix",      matrixGroup.viewProjection);
    _nontexturedGeometryProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjection);
    _nontexturedGeometryProgram->setMat3("u_NormalMatrix",              matrixGroup.normal);

    _lightVisualizationModel.draw();

    _matrixStack.popModel();
}

void Scene::_updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, _mode ? GL_FILL : GL_LINE); gl_bugcheck();
}

}
