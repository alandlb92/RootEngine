#pragma once

#include "Windows.h"
#include "Graphics/GraphicsMain.h"
#include "Core/Scene/SceneManager.h"
#include "Graphics/LightManager.h"
#include <memory>

namespace Faia
{
	namespace Root
	{
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
			std::unique_ptr<Graphics::Light::LightManager> _lightManager;

			static const float targetFramerate;
			static const float maxTimeStep;

		};
	}
}