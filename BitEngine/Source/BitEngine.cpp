#include "BitEngine.h"
#include "timeapi.h"
#include "FaiaInputSystem.h"
#include "GraphicsMain.h"

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
}


void BitEngine::Update()
{
    DWORD currentTime = timeGetTime();
    float deltaTime = (currentTime - _previousTime) / 1000.0f;
    _previousTime = currentTime;
    //deltaTime = std::min<float>(deltaTime, maxTimeStep);

    _graphicsMain->Update(deltaTime);
    _graphicsMain->Renderer();

    FaiaInputSystem::GetInstance()->Update(deltaTime);
}

void BitEngine::MoveCameraYFront(float deltaTime)
{
    _graphicsMain->_camera->AddPosition(Vector3D(0, 0, -100 * deltaTime));
}

void BitEngine::MoveCameraYBack(float deltaTime)
{
    _graphicsMain->_camera->AddPosition(Vector3D(0, 0, 100 * deltaTime));
}

void BitEngine::MoveCameraXRight(float deltaTime)
{
    _graphicsMain->_camera->AddPosition(Vector3D(100 * deltaTime, 0, 0));
}

void BitEngine::MoveCameraXLeft(float deltaTime)
{
    _graphicsMain->_camera->AddPosition(Vector3D(-100 * deltaTime, 0, 0));
}
