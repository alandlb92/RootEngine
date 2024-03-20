#include "BitEngine.h"
#include "timeapi.h"
#include "InputSystem.h"
#include "GraphicsMain.h"

const float BitEngine::targetFramerate = 30.0f;
const float BitEngine::maxTimeStep = 1.0f / targetFramerate;


BitEngine::BitEngine(HWND windowHandler) : _windowHandler(windowHandler)
{

}

void BitEngine::Init()
{
	_previousTime = timeGetTime();
	_graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
	_graphicsMain->Init();

	InputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::W, std::bind(&BitEngine::MoveCameraYFront, this, std::placeholders::_1));
	InputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::S, std::bind(&BitEngine::MoveCameraYBack, this, std::placeholders::_1));
}


void BitEngine::Update()
{
	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - _previousTime) / 1000.0f;
	_previousTime = currentTime;
	deltaTime = std::min<float>(deltaTime, maxTimeStep);

	_graphicsMain->Update(deltaTime);
	_graphicsMain->Renderer();

	InputSystem::GetInstance()->Update(currentTime);
}

void BitEngine::MoveCameraYFront(float deltaTime)
{
	_graphicsMain->_camera->AddPosition(Vector3D(0, 10 * deltaTime, 0));
}

void BitEngine::MoveCameraYBack(float deltaTime)
{
	_graphicsMain->_camera->AddPosition(Vector3D(0, -10 * deltaTime, 0));
}
