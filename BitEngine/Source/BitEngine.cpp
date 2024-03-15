#include "BitEngine.h"
#include "timeapi.h"


const float BitEngine::targetFramerate = 30.0f;
const float BitEngine::maxTimeStep = 1.0f / targetFramerate;


BitEngine::BitEngine(HWND windowHandler) : _windowHandler(windowHandler)
{}

void BitEngine::Init()
{
	_previousTime = timeGetTime();
	_graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
	_graphicsMain->Init();
}

void BitEngine::Update()
{
	DWORD currentTime = timeGetTime();
	float deltaTime = (currentTime - _previousTime) / 1000.0f;
	_previousTime = currentTime;
	deltaTime = std::min<float>(deltaTime, maxTimeStep);

	_graphicsMain->Update(deltaTime);
	_graphicsMain->Renderer();
}
