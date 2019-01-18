#include "Scene.hh"
#include "config.hh"


bool DepthTestEnabled = true;
bool FaceCullingEnabled = false;


Scene::Scene()
    : mode(true)
{
    ProgramBuilder builder;

    builder.addVertexShaderFromFile("resources/shaders/first_pass.vert");
    builder.addFragmentShaderFromFile("resources/shaders/first_pass.frag");

    firstPassProgram = builder.build();

    Texture2DLoader loader;

    loader.setFilename("resources/textures/container2.png");
    textureDiffuse = loader.load();

    firstPassProgram->use();
    VerticeFormat verticeFormat;
    verticeFormat.position = firstPassProgram->findAttribute("a_Position");
    verticeFormat.texture = firstPassProgram->findAttribute("a_Texture");


    sphereModel = make_shared<Model>();
    sphereModel->loadFromFile("resources/models/sphere.obj", verticeFormat);
    sphereModel->setPosition(vec3(-1.0f, 0.0f, 5.0f));
    sphereModel->setScale(vec3(0.05f, 0.05f, 0.05f));

    cubeModel = make_shared<Model>();
    cubeModel->loadFromFile("resources/models/cube.obj", verticeFormat);
    cubeModel->setPosition(vec3(1.0f, 0.0f, 5.0f));

    camera = make_shared<Camera>();
    camera->setPosition(vec3(0.0f, 0.0f, 0.0f));
    camera->setFrontVector(vec3(0.0f, 0.0f, 1.0f)); // look along +z axis, +x is forwarded to the left, +y is forwarded upwards

    projection = make_shared<Projection>();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    (DepthTestEnabled ? glEnable : glDisable)(GL_DEPTH_TEST);
    (FaceCullingEnabled ? glEnable : glDisable)(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    updateMode();

}

Scene::~Scene()
{
}

void Scene::draw()
{
    auto transformProjection = matrixStackSet.transformProjection(projection->getProjectionMatrix());
    auto transformView = matrixStackSet.transformView(camera->getViewMatrix());
    auto transformModel = matrixStackSet.transformModel(mat4());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_bugcheck();

    firstPassProgram->use();

    if (cubeModel)
        drawModel(*cubeModel);

    if (sphereModel)
        drawModel(*sphereModel);
}

void Scene::toggleMode()
{
    mode = !mode;
    updateMode();
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

void Scene::updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}

void Scene::updateUniforms()
{
    const auto & matrixGroup = matrixStackSet.getMatrixGroup();

    firstPassProgram->setMat4("u_MatrixModelViewProjection", matrixGroup.modelViewProjectionMatrix);

    const GLint unit = 0;
    textureDiffuse->use(unit);
    firstPassProgram->setTexture("u_DiffuseTexture", unit);
}

void Scene::drawModel(Model & model)
{
    auto transformModel = matrixStackSet.transformModel(model.getModelMatrix(), true);

    updateUniforms();
    model.draw();
}
