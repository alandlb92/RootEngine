#pragma once

#include "Windows.h"
#include "Graphics/RGraphics.h"
#include "Core/Scene/SceneManager.h"
#include "Graphics/DX11/LightManager_DX11.h"
#include <memory>

namespace Faia
{
	namespace Root
	{
		class RootEngine
		{

		public:
			RootEngine();

			void Init();
			void Update();

		private:
			DWORD _previousTime;
			//Todo: move this for a RenderData class or something like that
			std::unique_ptr<Graphics::Light::LightManager_DX11> _lightManager;
		};

		RootEngine* GetEngine();
	}
}