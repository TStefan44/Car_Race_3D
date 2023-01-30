#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
    delete track;
}


void Tema2::Init()
{
    srand(time(NULL));

    // Set camera 
    {
        perpectivaCamera = new implemented::Camera();
        perpectivaCamera->projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 1000.0f);

        orthoCamera = new implemented::Camera();
        float value = 150.f;

        orthoCamera->projectionMatrix = glm::ortho(-value, value, -value, value, 0.01f, 1000.0f);

        camera = perpectivaCamera;

        glm::ivec2 resolution = window->GetResolution();
        miniViewportArea = ViewportArea(resolution.x - 300, 50, resolution.x / 5.f, resolution.y / 5.f);
    }

    {
        Mesh* mesh = new Mesh("Skybox");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("Terrain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("CameraShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Track init
    {
        track = new tema2::Track(3, 3);
        Mesh* mesh = track->getComponentMeshes().front();
        meshes[mesh->GetMeshID()] = mesh;
    }
    
    // Forest init
    {
        forest = tema2::Forest(track);
        Mesh* mesh = new Mesh("FirTree");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "lowpolytree.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Cars init
    {
        carsNumber = 20;
        for (int i = 0; i < carsNumber; i++) {
            tema2::EnemyCar newCar;
            if (i % 2 == 0) {
                newCar = tema2::EnemyCar(tema2::DIR::FORWARD, track);
                newCar.setColor(glm::vec3(1, 0, 0));
            }
            else {
                newCar = tema2::EnemyCar(tema2::DIR::BACK, track);
                newCar.setColor(glm::vec3(1, 1, 0));
            }
            newCar.setCarSpeed(50.f + rand() % 101);
            newCar.setCurrentPoint(rand() % track->ARPData.size());

            cars.push_back(newCar);
        }

        Mesh* mesh = new Mesh("Car");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "car.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Player Car init
    {
        playercar = tema2::PlayerCar(glm::vec3(-289 * 3, 0, 26 * 3), camera, track);
        perpectivaCamera->Set(glm::vec3(-891.778f, 30.0335f , 19.4347f), playercar.getCurrentP(), glm::vec3(0, 1, 0));
        playercar.setCarSpeed(0.f); playercar.setColor(glm::vec3(0, 1, 1));
        playercar.setMaxSpeed(150.f); playercar.setAcc(1.f);
        playercar.update();
        carSlow = 1.f;
    }
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.6f, 0.81f, 0.91f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    {
        // Enemy Car logic
        for (int i = 0; i < carsNumber; i++) {
            cars[i].update(deltaTimeSeconds);
        }
    }

    {
        // Player Car logic
        if (playercar.carOnTrack() == true) {
            camera->MoveForward(playercar.getCarSpeed() * deltaTimeSeconds);
        }
        else {
            camera->MoveForward(-1.5f * playercar.getCarSpeed() * deltaTimeSeconds);
            playercar.setCarSpeed(0.f);
        }

        playercar.slowCar(carSlow);
        playercar.update(deltaTimeSeconds);
    }

    renderScene(deltaTimeSeconds);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::vec3 cameraPos = playercar.getCurrentP();
    orthoCamera->Set(cameraPos + glm::vec3(0, 100.f, 0), cameraPos, glm::vec3(0, 0, 1));
    camera = orthoCamera;

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    renderScene(deltaTimeSeconds);

    camera = perpectivaCamera;
}

void Tema2::renderScene(float deltaTimeSeconds) {
    {
        // Set Terrain
        glm::mat4 modelMatrix = glm::mat4(1);
        glm::vec3 carPosition = playercar.getCurrentP();
        modelMatrix = glm::translate(modelMatrix, glm::vec3(carPosition.x, 0, carPosition.z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));
        RenderSimpleMesh(meshes["Terrain"], shaders["CameraShader"], modelMatrix, glm::vec3(0, 1, 0));
    }

    {
        // Set Track
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["Track"], shaders["CameraShader"], modelMatrix, glm::vec3(0, 0, 0));
    }

    {
        // Set Forest
        std::vector<glm::mat4> matrixComp = forest.getMatrixComp();
        std::vector<tema2::treeType> trees = forest.getTreesType();
        for (int i = 0; i < matrixComp.size(); i++) {
            glm::mat4 modelMatrix = matrixComp[i];
            switch (trees[i]) {
            case tema2::treeType::FIR:
                RenderSimpleMesh(meshes["FirTree"], shaders["CameraShader"], modelMatrix, glm::vec3(0, 0, 1));
                break;
            }
        }
    }

    {
        // Set Cars
        glm::mat4 modelMatrix;
        for (int i = 0; i < carsNumber; i++) {
            modelMatrix = cars[i].getMatrixComp()[0];
            RenderSimpleMesh(meshes["Car"], shaders["CameraShader"], modelMatrix, cars[i].getColor());
        }
    }

    {   // Set Player Car
        glm::mat4 modelMatrix = playercar.getMatrixComp()[0];
        RenderSimpleMesh(meshes["Car"], shaders["CameraShader"], modelMatrix, playercar.getColor());
    }
}

void Tema2::FrameEnd()
{

}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    GLint loc_object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(loc_object_color, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();;
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = camera->projectionMatrix;
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Send car position to shader
    glm::vec3 carPosition = playercar.getCurrentP();
    GLint loc_pos_car = glGetUniformLocation(shader->program, "pos_car");
    glUniform3fv(loc_pos_car, 1, glm::value_ptr(carPosition));

    GLint loc_scaleFactor = glGetUniformLocation(shader->program, "scaleFactor");
    glUniform1f(loc_scaleFactor, scaleFactor);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int location1 = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int location2 = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int location3 = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = camera->projectionMatrix;
    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
        float cameraSpeed = playercar.getCarSpeed();
        float carAcc = playercar.getAcc();
        float rotateSpeed = (cameraSpeed != 0) ? abs(cameraSpeed) / 50 : carAcc;

        for (int i = 0; i < carsNumber; i++) {
            if (playercar.checkCollision(cars[i]) == true) {
                playercar.setCarSpeed(0.f);
                return;
            }
        }

        if (window->KeyHold(GLFW_KEY_W)) {
            // Translate the camera forward
            playercar.accelerateCar(carAcc + carSlow);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->RotateThirdPerson_OY(rotateSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // Translate the camera backward
            playercar.accelerateCar(-carAcc - carSlow);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // Translate the camera to the right
            camera->RotateThirdPerson_OY(-rotateSpeed * deltaTime);
        }
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
