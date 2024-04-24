#pragma once

#include <memory>
#include "Windows.h"
#include "Graphics/GraphicsMain.h"
#include "Core/Scene/SceneManager.h"

class BitEngine
{

public:
	BitEngine(HWND windowHandler);

	void Init();
	void Update();

private:
	DWORD _previousTime;
	HWND _windowHandler;

	std::unique_ptr<GraphicsMain> _graphicsMain;
	std::unique_ptr<SceneManager> _sceneManager;

	static const float targetFramerate;
	static const float maxTimeStep;

};