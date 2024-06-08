#pragma once

#include <mat4.h>
using namespace Core;
using namespace Core::Maths;

#include <transform.h>
using namespace Physics;

#include <component.h>
using namespace Engine;

namespace Engine
{
    class Player;
}

namespace LowRenderer
{
    class Camera : public Component
    {
    protected:
        bool followPlayer = true;
        void followMode();
        void inputMode();
        vec3 offset = { 0, 3, 5 };
        vec3 anchorPoint = { 0, 0, 0 };

        Player* player = nullptr;

        struct CameraInputs
        {
            bool mouseCaptured = false;

            float mouseX = 0.f;
            float mouseY = 0.f;

            float aspect = 0.0f;

            float deltaX = 0.f;
            float deltaY = 0.f;
            bool moveForward = false;
            bool moveBackward = false;
            bool moveRight = false;
            bool moveLeft = false;
            bool moveUp = false;
            bool moveDown = false;
            bool moveFaster = false;
        };

    public:
        //// Controls
        float moveSpeed = 10.0f;
        //
        //// Transform
        float yaw = 0.f;
        float pitch = 0.f;
        //vec3 position = { 0.0f, 0.0f, 5.0f };

		CameraInputs cInputs = CameraInputs();

        // Projection
        float aspect = 1.f;
        float fovY = 60.f * DEG2RAD;
        float near = 0.01f;
        float far = 1000.f;
        bool isOrthographic = false;
        float orthoSize = 10.f;

        int layer = 0;
        mat4 viewProj;

        Camera() = default;
        Camera(GameObject* go);
        ~Camera() override;

        void update() override;
		void start() override;
        mat4 getViewMatrix();
        mat4 getProjection();

        void mouseMovements(bool value);

        // IMGUI INTERFACE
        //void showImGuiControls();

        void virtual drawEditorWindow() override;

        std::string getSaveFormat() override;
        void loadFromSaveFormat(const std::string& line) override;
    };
}