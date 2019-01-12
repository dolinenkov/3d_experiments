#include "Scene.hh"
#include "config.hh"


Scene::Scene(): mode(false)
{
    ProgramBuilder builder;

    builder.addVertexShaderFromFile("first_pass.vert");
    builder.addFragmentShaderFromFile("first_pass.frag");

    firstPassProgram = builder.build();

    Texture2DLoader loader;
    loader.setFilename("container2.png");
    textureDiffuse = loader.load();

    loader.setFilename("container2_specular.png");
    textureSpecular = loader.load();

    sphereModel = make_shared<Model>();
    //sphereModel->loadFromFile("sphere.obj");
    sphereModel->loadFromFile("cube.obj");

    format = make_shared<VertexFormat>();
    format->init(firstPassProgram);

    cameraPosition = vec3(0.0f, 0.0f, -10.0f);
    cameraDirection = vec3(0.0f, 0.0f, 1.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); gl_bugcheck();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    updateMode();
}

Scene::~Scene()
{
}

void Scene::draw()
{
    cameraDirection = vec3(cosf(radians(cameraAngles[0])), 0.0f, cosf(radians(cameraAngles[1])));

    projectionStack.push(perspective(radians(60.0f), 1.5f, 0.1f, 100.0f));
    viewStack.push(lookAt(cameraPosition, cameraPosition + cameraDirection, vec3(0.0f, 1.0f, 0.0f)));
    modelStack.push(mat4());

    mat3 normalMatrix(transpose(inverse(modelStack.top())));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_bugcheck();

    mat4 mvp = projectionStack.top() * viewStack.top() * modelStack.top();
    firstPassProgram->use();

    firstPassProgram->setMat4("u_MatrixModelViewProjection", mvp);
    firstPassProgram->setMat4("u_MatrixModel", modelStack.top());
    firstPassProgram->setMat3("u_MatrixNormal", normalMatrix);

    // camera

    firstPassProgram->setVec3("u_Camera.position", cameraPosition);

    // light source

    firstPassProgram->setVec3("u_Light.position", vec3(10.0f, -2.0f, 2.0f));

    firstPassProgram->setVec3("u_Light.color", vec3(1.0f, 1.0f, 1.0f));

    firstPassProgram->setFloat("u_Light.intensityAmbient", 0.0f);
    firstPassProgram->setFloat("u_Light.intensityDiffuse", 0.3f);
    firstPassProgram->setFloat("u_Light.intensitySpecular", 1.0f);

    // material

    firstPassProgram->setFloat("u_Material.shininess", 0.3f);
    textureDiffuse->use(0);
    firstPassProgram->setTexture("u_Material.textureDiffuse", 0);
    textureSpecular->use(1);
    firstPassProgram->setTexture("u_Material.textureSpecular", 1);


    format->activate();

    sphereModel->draw();

    while (!projectionStack.empty())
        projectionStack.pop();

    while (!viewStack.empty())
        viewStack.pop();

    while (!modelStack.empty())
        modelStack.pop();
}

void Scene::moveX(bool _positive)
{
    cameraPosition.x += 0.1f * (_positive ? 1.0f : -1.0f);
}

void Scene::moveY(bool _positive)
{
    cameraPosition.y += 0.1f * (_positive ? 1.0f : -1.0f);
}

void Scene::moveZ(bool _positive)
{
    cameraPosition.z += 0.1f * (_positive ? 1.0f : -1.0f);
}

void Scene::rotateX(float _d)
{
    //cameraAngles[0] += 720.0f * _d;
}

void Scene::rotateZ(float _d)
{
    //cameraAngles[1] += 720.0f * _d;
}

void Scene::toggleMode()
{
    mode = !mode;
    updateMode();
}

void Scene::updateMode()
{
    glPolygonMode(GL_FRONT_AND_BACK, mode ? GL_FILL : GL_LINE); gl_bugcheck();
}
