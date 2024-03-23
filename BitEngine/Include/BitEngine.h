#pragma once

#include <memory>
#include "Windows.h"
#include "GraphicsMain.h"

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
	void MoveCameraYFront(float deltaTime);
	void MoveCameraYBack(float deltaTime);
	void MoveCameraXRight(float deltaTime);
	void MoveCameraXLeft(float deltaTime);


	static const float targetFramerate;
	static const float maxTimeStep;

};