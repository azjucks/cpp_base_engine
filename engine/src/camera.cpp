#include <camera.h>
using namespace LowRenderer;

#include <time_manager.h>

#include <input.h>
#include <imgui.h>
#include <functional>

#include <graph.h>
#include <scene.h>
#include <player.h>
#include <maths.h>

#include <renderer.h>

#include <gameobject.h>

Camera::Camera(GameObject* go)
    : Component(go)
{
    Renderer::instance()->registerCamera(this);
}

Camera::~Camera()
{
    Renderer::instance()->unregisterCamera(this);
    Component::~Component();
}

/*
Camera::Camera(int width, int height)
{
    // width and height are useful to compute projection matrix with the right aspect ratio
    aspect = (float)width / (float)height;
}*/

void LowRenderer::Camera::mouseMovements(bool value)
{
    GLFWwindow* window = Input::instance()->window;

    cInputs.mouseCaptured = value;
    glfwSetInputMode(window, GLFW_CURSOR, value == true ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    if (cInputs.mouseCaptured)
    {
        double newMouseX, newMouseY;
        glfwGetCursorPos(window, &newMouseX, &newMouseY);
        cInputs.mouseX = (float)newMouseX;
        cInputs.mouseY = (float)newMouseY;
    }
}

void LowRenderer::Camera::start()
{
    Resources::Scene* currentScene = Core::DataStructure::Graph::instance()->getCurrentScene();
    GameObject* go = (GameObject*)currentScene->findGameObjectWithName("Player");
    if (go == nullptr)
    {
        followPlayer = false;
        return;
    }

    player = go->GetComponent<Player>();

    if (player == nullptr)
    {
        followPlayer = false;
        return;
    }

    // Binding all the actions to their keys for the GLFW callbacks
	Input::instance()->actionsPressed.find(GLFW_KEY_UP)->second = [&] { cInputs.moveForward = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_DOWN)->second = [&] { cInputs.moveBackward = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_LEFT)->second = [&] { cInputs.moveLeft = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_RIGHT)->second = [&] { cInputs.moveRight = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_LEFT_SHIFT)->second = [&] { cInputs.moveUp = true; };
	Input::instance()->actionsPressed.find(GLFW_KEY_LEFT_CONTROL)->second = [&] { cInputs.moveDown = true; };

	Input::instance()->actionsReleased.find(GLFW_KEY_UP)->second = [&] { cInputs.moveForward = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_DOWN)->second = [&] { cInputs.moveBackward = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_LEFT)->second = [&] { cInputs.moveLeft = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_RIGHT)->second = [&] { cInputs.moveRight = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_LEFT_SHIFT)->second = [&] { cInputs.moveUp = false; };
	Input::instance()->actionsReleased.find(GLFW_KEY_LEFT_CONTROL)->second = [&] { cInputs.moveDown = false; };

    Input::instance()->mousePressed.find(GLFW_MOUSE_BUTTON_RIGHT)->second = [&] { Camera::mouseMovements(true); };

    Input::instance()->mouseReleased.find(GLFW_MOUSE_BUTTON_RIGHT)->second = [&] { Camera::mouseMovements(false); };
}

void Camera::inputMode()
{
    GLFWwindow* window = Input::instance()->window;

    const float MOUSE_SENSITIVITY = 0.001f;

    cInputs.deltaX = 0;
    cInputs.deltaY = 0;

    if (cInputs.mouseCaptured)
    {
        double newMouseX, newMouseY;

        float mouseDeltaX = 0.f;
        float mouseDeltaY = 0.f;

        glfwGetCursorPos(window, &newMouseX, &newMouseY);
        mouseDeltaX = (float)(newMouseX - cInputs.mouseX);
        mouseDeltaY = (float)(newMouseY - cInputs.mouseY);
        cInputs.mouseX = (float)newMouseX;
        cInputs.mouseY = (float)newMouseY;

        cInputs.deltaX = mouseDeltaX;
        cInputs.deltaY = mouseDeltaY;
    }

    yaw += cInputs.deltaX * MOUSE_SENSITIVITY;
    pitch += cInputs.deltaY * MOUSE_SENSITIVITY;

    float speed = moveSpeed;
    if (cInputs.moveFaster)
        speed *= 2;

    float deltaTime = TimeManager::instance()->deltaTime;

    float forwardMovement = 0.f;
    if (cInputs.moveForward)
        forwardMovement += speed * deltaTime;
    if (cInputs.moveBackward)
        forwardMovement -= speed * deltaTime;

    float strafeMovement = 0.f;
    if (cInputs.moveRight)
        strafeMovement += speed * deltaTime;
    if (cInputs.moveLeft)
        strafeMovement -= speed * deltaTime;

    float upwardMovement = 0.f;
    if (cInputs.moveUp)
        upwardMovement += speed * deltaTime;
    if (cInputs.moveDown)
        upwardMovement -= speed * deltaTime;

    gameObject->transform->position.x += sinf(yaw) * forwardMovement;
    gameObject->transform->position.z -= cosf(yaw) * forwardMovement;

    gameObject->transform->position.x += cosf(-yaw) * strafeMovement;
    gameObject->transform->position.z -= sinf(-yaw) * strafeMovement;

    gameObject->transform->position.y += upwardMovement;

    gameObject->transform->rotation = vec3{ pitch, yaw, 0.f };
}

void Camera::followMode()
{
    Transform* pTransform = player->gameObject->transform.get();

    anchorPoint = Maths::lerp(anchorPoint, pTransform->position + offset, 0.05f);
    gameObject->transform->position = anchorPoint;
}

void Camera::update()
{
    GLFWwindow* window = Input::instance()->window;

    this->aspect = cInputs.aspect;

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    cInputs.aspect = (float)width / (float)height;

    if (followPlayer)
        followMode();
    else
        inputMode();
}

mat4 Camera::getViewMatrix()
{
    //return gameObject->transform->getModelUpdated();
    return rotateX(gameObject->transform->rotation.x)* rotateY(gameObject->transform->rotation.y)* translate(-gameObject->transform->position);
}

mat4 Camera::getProjection()
{
    if (isOrthographic)
        return orthographic(orthoSize, aspect, near, far);
    else
        return perspective(fovY, aspect, near, far);
}

void Camera::drawEditorWindow()
{
    if (ImGui::TreeNode("Component: Camera"))
    {
        ImGui::Checkbox("Follow player", &followPlayer);
        if (followPlayer)
        {
            ImGui::DragFloat3("Height offset", offset.e);
        }

        ImGui::Checkbox("Is Orthographic ?", &isOrthographic);
        ImGui::SliderFloat("SPEED", &moveSpeed, 1.0f, 100.0f);

        static float fov = 60.0f;

        if (isOrthographic)
        {
            ImGui::SliderFloat("Ortho Size", &orthoSize, 0, 500);
        }
        else
        {
            ImGui::SliderFloat("FOV", &fov, 0.1f, 280.0f);
            fovY = fov * DEG2RAD;
        }


        ImGui::SliderFloat("Near", &near, 0.001f, 50.0f);
        ImGui::SliderFloat("Far ", &far, 10.0f, 2000.0f);

        ImGui::TreePop();
    }
}

std::string Camera::getSaveFormat()
{
    return "component " + std::to_string((int)ComponentType::CAMERA) + " "
        + std::to_string(moveSpeed) + " "
        + std::to_string(yaw) + " "
        + std::to_string(pitch) + " "
        + std::to_string(fovY) + " "
        + std::to_string(near) + " "
        + std::to_string(far) + " "
        + std::to_string(isOrthographic) + " "
        + std::to_string(orthoSize) + " "
        + std::to_string(layer) + " "
        + std::to_string(offset.x) + " "
        + std::to_string(offset.y) + " "
        + std::to_string(offset.z) + " "
        + "\n";
}

void Camera::loadFromSaveFormat(const std::string& line)
{
    std::istringstream iss(line);

    std::string component;
    iss >> component;
    iss >> component;

    iss >> moveSpeed;
    iss >> yaw;
    iss >> pitch;
    iss >> fovY;
    iss >> near;
    iss >> far;
    iss >> isOrthographic;
    iss >> orthoSize;
    iss >> layer;
    iss >> offset.x;
    iss >> offset.y;
    iss >> offset.z;
}