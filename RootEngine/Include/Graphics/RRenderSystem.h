#pragma once

#define FAIA_GRAPHICS_DX12 = 0;
#define FAIA_GRAPHICS_DX11 = 0;

#if defined(FAIA_GRAPHICS_DX12)
	#include "DX12/Graphics_DX12.h"
#elif defined(FAIA_GRAPHICS_DX11)
	#include "DX11/GraphicsMain_DX11.h"
#else
	#error FAIA GRAPHICS NOT defined!
#endif


#include <functional>
#include <memory>

namespace Faia
{
	namespace Root
	{
		namespace Graphics
		{
			using PostRenderFunction = std::function<void()>;

			class RRendererSystem
			{
			public:
				RRendererSystem();
				void Update();
				void RegisterPostRendererFunction(PostRenderFunction postRendererFunction);
				RIGraphicsPlatform* GetGraphicsPlatform();

			private:
				std::unique_ptr<RIGraphicsPlatform> mGraphicsPlatform = {};
				std::vector<PostRenderFunction> mPostRenderFunctions = {};

			};

			RRendererSystem* GetRenderSystem();
			inline void InitializeRenderSystem()
			{
				GetRenderSystem();
			}
		}

	}
}