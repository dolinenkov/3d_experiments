#include "Scene.hh"
#include "config.hh"


//



//


Scene::Scene(): mode(false)
{
    ProgramBuilder builder;

    builder.addVertexShaderFromFile("resources/shaders/first_pass.vert");
    builder.addFragmentShaderFromFile("resources/shaders/first_pass.frag");

    firstPassProgram = builder.build();

    Texture2DLoader loader;
    loader.setFilename("resources/textures/container2.png");
    textureDiffuse = loader.load();

    loader.setFilename("resources/textures/container2_specular.png");
    textureSpecular = loader.load();

    sphereModel = make_shared<Model>();
    sphereModel->loadFromFile("resources/models/sphere.obj");
    //sphereModel->loadFromFile("cube.obj");

    format = make_shared<VertexFormat>();
    format->init(firstPassProgram);

    camera = make_shared<Camera>();
    camera->setPosition(vec3(0.0f, 2.0f, 10.0f));
    camera->setFrontVector(vec3(0.0f, 0.0f, 0.0f));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); gl_bugcheck();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    mode = false;
    updateMode();


    matrixStackSet.getProjectionMatrixStack().push(perspective(radians(60.0f), 1.5f, 0.1f, 100.0f));
    matrixStackSet.getModelMatrixStack().push(mat4());
}

Scene::~Scene()
{
}

void Scene::draw()
{
    matrixStackSet.getViewMatrixStack().push(camera->getViewMatrix());
    const auto & matrixGroup = matrixStackSet.getMatrixGroup();

    //SDL_Log("camera: %f, %f, %f\n", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_bugcheck();

    firstPassProgram->use();

    firstPassProgram->setMat4("u_MatrixModelViewProjection", matrixGroup.modelViewProjectionMatrix);
    firstPassProgram->setMat4("u_MatrixModel", matrixGroup.modelMatrix);
    firstPassProgram->setMat3("u_MatrixNormal", matrixGroup.normalMatrix);

    // camera

    firstPassProgram->setVec3("u_Camera.position", camera->getPosition());

    // light source

    //firstPassProgram->setVec3("u_Light.position", vec3(10.0f, -2.0f, 2.0f));
    firstPassProgram->setVec3("u_Light.position", -camera->getPosition());

    firstPassProgram->setVec3("u_Light.color", vec3(1.0f, 1.0f, 1.0f));

    firstPassProgram->setFloat("u_Light.intensityAmbient", 0.2f);
    firstPassProgram->setFloat("u_Light.intensityDiffuse", 1.0f);
    firstPassProgram->setFloat("u_Light.intensitySpecular", 1.0f);

    // material

    firstPassProgram->setFloat("u_Material.shininess", 1.0f);
    textureDiffuse->use(0);
    firstPassProgram->setTexture("u_Material.textureDiffuse", 0);
    textureSpecular->use(1);
    firstPassProgram->setTexture("u_Material.textureSpecular", 1);

    format->activate();

    sphereModel->getModelMatrix();

    sphereModel->draw();

    matrixStackSet.getViewMatrixStack().pop();
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

void Scene::updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}
