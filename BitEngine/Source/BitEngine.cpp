#include "BitEngine.h"
#include "timeapi.h"
#include "InputSystem.h"

const float BitEngine::targetFramerate = 30.0f;
const float BitEngine::maxTimeStep = 1.0f / targetFramerate;


BitEngine::BitEngine(HWND windowHandler) : _windowHandler(windowHandler)
{}

void BitEngine::Init()
{
	_previousTime = timeGetTime();
	_graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
	_graphicsMain->Init();

	InputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_PRESSED, KeyCode::A, std::bind(&Camera::StartRotate, &_graphicsMain->_camera));
	InputSystem::GetInstance()->RegisterActionEvent(InputEventType::KEY_RELEASED, KeyCode::A, std::bind(&Camera::StopRotate, &_graphicsMain->_camera));
}

void BitEngine::Update()
{
	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - _previousTime) / 1000.0f;
	_previousTime = currentTime;
	deltaTime = std::min<float>(deltaTime, maxTimeStep);

	_graphicsMain->Update(deltaTime);
	_graphicsMain->Renderer();

	InputSystem::GetInstance()->Update(deltaTime);
}