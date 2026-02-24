#include "Graphics/RRenderSystem.h"
#include "Faia/Debug.h"

namespace Faia
{
	namespace Root
	{
        namespace Graphics
        {
			RRendererSystem::RRendererSystem()
			{
				if (mGraphicsPlatform == nullptr)
				{
					mGraphicsPlatform = std::make_unique<GraphicsPlatform>();	
					mGraphicsPlatform->Setup();
				}
			}
			
			void RRendererSystem::Update()
			{
				mGraphicsPlatform->BeginFrame();
				for (PostRenderFunction& postRenderFunc : mPostRenderFunctions)
				{
					postRenderFunc();
				}
				mGraphicsPlatform->EndFrame();
			}

			void RRendererSystem::RegisterPostRendererFunction(PostRenderFunction postRendererFunction)
			{
				mPostRenderFunctions.push_back(postRendererFunction);
			}

			RIGraphicsPlatform* RRendererSystem::GetGraphicsPlatform()
			{
				return mGraphicsPlatform.get();
			}

			RRendererSystem* gRenderSystem = nullptr;
			RRendererSystem* GetRenderSystem()
			{
				if (!gRenderSystem)
				{
					gRenderSystem = new RRendererSystem();
				}

				return gRenderSystem;
			}
		}
	}
}