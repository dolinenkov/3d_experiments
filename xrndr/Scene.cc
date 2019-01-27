#include <xrndr/Scene.hh>
#include <xrndr/Texture2DLoader.hh>


namespace xrndr
{

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

    builder.addVertexShaderFromFile("postprocess.vert");
    builder.addFragmentShaderFromFile("postprocess.frag");
    _postprocessProgram = builder.build();

    //

    VerticeFormat geometryPassVerticeFormat;
    geometryPassVerticeFormat.position = _firstPassProgram->findAttribute("a_Position");
    geometryPassVerticeFormat.texture = _firstPassProgram->findAttribute("a_Texture");
    geometryPassVerticeFormat.normal = _firstPassProgram->findAttribute("a_Normal");

    VerticeFormat nontexturedGeometryVerticeFormat;
    nontexturedGeometryVerticeFormat.position = _nontexturedGeometryProgram->findAttribute("a_Position");

    VerticeFormat postprocessVerticeFormat;
    postprocessVerticeFormat.position = _postprocessProgram->findAttribute("a_Position");
    postprocessVerticeFormat.texture = _postprocessProgram->findAttribute("a_Texture");

    //

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 0.0f, 3.0f);
    _pointLights.back().color       = vec3(1.0f, 1.0f, 1.0f);
    _pointLights.back().attenuation = vec3(1.0f, 0.0f, 0.3f);
    _pointLights.back().intensity   = vec3(0.0f, 0.3f, 0.3f);

    _pointLights.emplace_back();
    _pointLights.back().position    = vec3(0.0f, 1.0f, -2.0f);
    _pointLights.back().color       = vec3(1.0f, 0.3f, 0.3f);
    _pointLights.back().attenuation = vec3(1.0f, 0.0f, 0.5f);
    _pointLights.back().intensity   = vec3(0.0f, 0.7f, 0.5f);

    _directedLights.emplace_back();
    _directedLights.back().direction = vec3(-1.0f, 0.0f, -1.0f);
    _directedLights.back().color     = vec3(1.0f, 1.0f, 1.0f);
    _directedLights.back().intensity = vec3(0.0f, 0.2f, 0.2f);

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

    const uint32_t index[] = { 0, 1, 2, 2, 3, 0 };

    const Vertice vertex[] =
    {
        { {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} },
        { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
        { {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f} },
        { {1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
    };

    MeshData md;
    md.format = &postprocessVerticeFormat;
    md.vertexData = vertex;
    md.vertexSize = 4;
    md.indexData = index;
    md.indexSize = 6;

    _screenQuad.init(md);

    _camera = make_shared<Camera>();
    _camera->setPosition(vec3(0.0f, 0.0f, 0.5f));
    _camera->setFrontVector(vec3(0.0f, 0.0f, 1.0f)); // look along +z axis, +x is forwarded to the left, +y is forwarded upwards

    _projection = make_shared<Projection>();

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    glGenFramebuffers(1, &_framebuffer); gl_bugcheck();
    glGenTextures(1, &_framebufferTexture); gl_bugcheck();
    glGenRenderbuffers(1, &_framebufferRenderbuffer); gl_bugcheck();

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer); gl_bugcheck();

    updateViewport(1920, 1080);
}

Scene::~Scene()
{
    glDeleteTextures(1, &_framebufferTexture); gl_bugcheck();
    glDeleteFramebuffers(1, &_framebuffer); gl_bugcheck();
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
    _pass = RendererPass::Geometry;

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer); gl_bugcheck();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        assert(false);
    }
    gl_bugcheck();

    glEnable(GL_DEPTH_TEST); gl_bugcheck();
    glEnable(GL_CULL_FACE); gl_bugcheck();
    glDisable(GL_STENCIL_TEST); gl_bugcheck();

    glPolygonMode(GL_FRONT_AND_BACK, _mode ? GL_FILL : GL_LINE); gl_bugcheck();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); gl_bugcheck();

    _matrixStack.pushProjection(_projection->getProjectionMatrix());
    _matrixStack.pushView(_camera->getViewMatrix());

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

    //

    for (auto & model : _models)
    {
        _matrixStack.pushModel(model.getModelMatrix(), true);

        const auto & matrixGroup = _matrixStack.getCache();

        _firstPassProgram->setMat4("u_ModelMatrix", matrixGroup.model);
        _firstPassProgram->setMat4("u_ViewMatrix", matrixGroup.view);
        _firstPassProgram->setMat4("u_ProjectionMatrix", matrixGroup.projection);
        _firstPassProgram->setMat4("u_ModelViewMatrix", matrixGroup.modelView);
        _firstPassProgram->setMat4("u_ViewProjectionMatrix", matrixGroup.viewProjection);
        _firstPassProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjection);
        _firstPassProgram->setMat3("u_NormalMatrix", matrixGroup.normal);

        model.draw(*this);

        _matrixStack.popModel();
    }

    //

    _pass = RendererPass::Debug;

    _nontexturedGeometryProgram->use();

    for (auto & pointLight : _pointLights)
    {
        _lightVisualizationModel.setPosition(pointLight.position);

        _matrixStack.pushModel(_lightVisualizationModel.getModelMatrix(), true);

        const auto & matrixGroup = _matrixStack.getCache();

        _nontexturedGeometryProgram->setVec3("u_Color", pointLight.color);

        _nontexturedGeometryProgram->setMat4("u_ModelMatrix", matrixGroup.model);
        _nontexturedGeometryProgram->setMat4("u_ViewMatrix", matrixGroup.view);
        _nontexturedGeometryProgram->setMat4("u_ProjectionMatrix", matrixGroup.projection);
        _nontexturedGeometryProgram->setMat4("u_ModelViewMatrix", matrixGroup.modelView);
        _nontexturedGeometryProgram->setMat4("u_ViewProjectionMatrix", matrixGroup.viewProjection);
        _nontexturedGeometryProgram->setMat4("u_ModelViewProjectionMatrix", matrixGroup.modelViewProjection);
        _nontexturedGeometryProgram->setMat3("u_NormalMatrix", matrixGroup.normal);

        _lightVisualizationModel.draw(*this);

        _matrixStack.popModel();
    }

    _matrixStack.popProjection();
    _matrixStack.popView();


    _pass = RendererPass::Postprocess;

    glBindFramebuffer(GL_FRAMEBUFFER, 0); gl_bugcheck();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); gl_bugcheck();

    glDisable(GL_DEPTH_TEST); gl_bugcheck();
    glDisable(GL_STENCIL_TEST); gl_bugcheck();
    glDisable(GL_CULL_FACE); gl_bugcheck();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); gl_bugcheck();
    glViewport(0, 0, _width, _height);

    _postprocessProgram->use();

    _postprocessProgram->setTexture("u_Texture", 0);
    _postprocessProgram->setFloat("u_Gamma", 2.2f);
    _postprocessProgram->setFloat("u_Exposure", 2.0f);

    glActiveTexture(GL_TEXTURE0); gl_bugcheck();
    glBindTexture(GL_TEXTURE_2D, _framebufferTexture); gl_bugcheck();

    _screenQuad.draw();


    _pass = RendererPass::None;
}

void Scene::toggleMode()
{
    _mode = !_mode;
}

shared_ptr<Camera> Scene::getCamera()
{
    return _camera;
}

void Scene::updateViewport(int width, int height)
{
    if (width > 0 && height > 0)
    {
        _width = width;
        _height = height;

        _projection->makePerspective(60.0, static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer); gl_bugcheck();
        glViewport(0, 0, width, height); gl_bugcheck();

        glBindTexture(GL_TEXTURE_2D, _framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); gl_bugcheck();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); gl_bugcheck();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); gl_bugcheck();

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _framebufferTexture, 0); gl_bugcheck();

        glBindRenderbuffer(GL_RENDERBUFFER, _framebufferRenderbuffer); gl_bugcheck();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); gl_bugcheck();

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _framebufferRenderbuffer); gl_bugcheck();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); gl_bugcheck();
        glViewport(0, 0, width, height); gl_bugcheck();
    }
}

void Scene::setMaterial(Material * material)
{
    switch (_pass)
    {
    case RendererPass::Geometry:

        _firstPassProgram->setTexture("u_Material.diffuseTexture", 0);
        _firstPassProgram->setTexture("u_Material.specularTexture", 1);

        if (material)
        {
            auto diffuseTexture = material->diffuseTexture;
            auto specularTexture = material->specularTexture;

            _firstPassProgram->setFloat("u_Material.shininess", material->shininess);

            if (diffuseTexture)
                diffuseTexture->use(0);
            else
                Texture2D::unbind(0);

            if (specularTexture)
                specularTexture->use(1);
            else
                Texture2D::unbind(1);
        }
        break;

    default:
        break;
    }
}

}
