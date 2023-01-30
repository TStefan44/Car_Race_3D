#pragma once
#include "components/simple_scene.h"
#include "lab_m1/Tema2/headers/lab_camera.h"
#include "headers/forest.h"
#include "headers/enemy_car.h"
#include "headers/player_car.h"

using namespace std;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void renderScene(float deltaTimeSeconds);

     protected:
        // Camera varaibles 
        implemented::Camera *camera, *perpectivaCamera, *orthoCamera;
        ViewportArea miniViewportArea;

        // Track and Forest variables
        tema2::Track *track;
        tema2::Forest forest;

        // Cars variables
        std::vector<tema2::EnemyCar> cars;
        tema2::PlayerCar playercar;
        int carsNumber;
        float carSlow;
        
        float scaleFactor = 0.001f;
    };
}   // namespace m1
