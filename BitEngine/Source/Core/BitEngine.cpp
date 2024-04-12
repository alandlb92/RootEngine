#include "Core/BitEngine.h"
#include "Data/Vector3D.h"
#include "timeapi.h"
#include "FaiaInputSystem.h"

const float BitEngine::targetFramerate = 30.0f;
const float BitEngine::maxTimeStep = 1.0f / targetFramerate;

using namespace Faia::InputSystem;

BitEngine::BitEngine(HWND windowHandler) : _windowHandler(windowHandler)
{

}

void BitEngine::Init()
{
    _previousTime = timeGetTime();
    _graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
    _graphicsMain->Init();

    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::W, std::bind(&BitEngine::MoveCameraYFront, this, std::placeholders::_1));
    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::S, std::bind(&BitEngine::MoveCameraYBack, this, std::placeholders::_1));
    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::A, std::bind(&BitEngine::MoveCameraXRight, this, std::placeholders::_1));
    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::D, std::bind(&BitEngine::MoveCameraXLeft, this, std::placeholders::_1));
    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::Q, std::bind(&BitEngine::MoveCameraZUp, this, std::placeholders::_1));
    FaiaInputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::E, std::bind(&BitEngine::MoveCameraZDown, this, std::placeholders::_1));

    FaiaInputSystem::GetInstance()->RegisterAxisEvent(AxisType::MOUSE_X, std::bind(&BitEngine::RotateCameraX, this, std::placeholders::_1, std::placeholders::_2));
    FaiaInputSystem::GetInstance()->RegisterAxisEvent(AxisType::MOUSE_Y, std::bind(&BitEngine::RotateCameraY, this, std::placeholders::_1, std::placeholders::_2));
}


void BitEngine::Update()
{
    DWORD currentTime = timeGetTime();
    float deltaTime = (currentTime - _previousTime) / 1000.0f;
    _previousTime = currentTime;
    //deltaTime = std::min<float>(deltaTime, maxTimeStep);

    //TODO: I need to make the render a separeted thread or the vsync will stop my application and i will have some problems with Input system events for exemple. 
    _graphicsMain->Update(deltaTime);
    _graphicsMain->Renderer();

    FaiaInputSystem::GetInstance()->Update(deltaTime);
}

void BitEngine::MoveCameraYFront(float deltaTime)
{
    _graphicsMain->_camera->AddLocalPosition(Vector3D(0, 0, 100 * deltaTime));
}

void BitEngine::MoveCameraYBack(float deltaTime)
{
    _graphicsMain->_camera->AddLocalPosition(Vector3D(0, 0, -100 * deltaTime));
}

void BitEngine::MoveCameraXRight(float deltaTime)
{
    _graphicsMain->_camera->AddLocalPosition(Vector3D(-100 * deltaTime, 0, 0));
}

void BitEngine::MoveCameraXLeft(float deltaTime)
{
    _graphicsMain->_camera->AddLocalPosition(Vector3D(100 * deltaTime, 0, 0));
}

void BitEngine::MoveCameraZUp(float deltaTime)
{
    _graphicsMain->_camera->AddWorldPosition(Vector3D(0, 100 * deltaTime, 0));
}

void BitEngine::MoveCameraZDown(float deltaTime)
{
    _graphicsMain->_camera->AddWorldPosition(Vector3D(0, -100 * deltaTime, 0));
}

void BitEngine::RotateCameraX(float axisValue, float deltaTime)
{
    _graphicsMain->_camera->Rotate(0, axisValue * deltaTime * 300, 0);
}

void BitEngine::RotateCameraY(float axisValue, float deltaTime)
{
    _graphicsMain->_camera->Rotate(axisValue * deltaTime * 300, 0, 0);
}
