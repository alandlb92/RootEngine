#include "Core/RootEngine.h"
#include "FaiaInputSystem.h"

namespace Faia
{
    namespace Root
    {
        using namespace Faia::InputSystem;
        using namespace Graphics;

        RootEngine* gEngine;
        RootEngine* GetEngine()
        {
            if (gEngine == nullptr)
            {
                gEngine = new RootEngine();
                gEngine->Init();
            }

            return gEngine;
        }

        RootEngine::RootEngine()
        {}

        void RootEngine::Init()
        {
            _lightManager = std::make_unique<Graphics::Light::LightManager_DX11>();
            _previousTime = timeGetTime();
            GetGraphics()->SetupDevice();
            GetSceneManager()->LoadScene(MakeDemoScene());
        }


        void RootEngine::Update()
        {
            DWORD currentTime = timeGetTime();
            float deltaTime = (currentTime - _previousTime) / 1000.0f;
            _previousTime = currentTime;
            GetFaiaInputSystem()->Update(deltaTime);
            GetSceneManager()->Update(deltaTime);
            //TODO: I need to make the render a separeted thread or the vsync will stop my application and i will have some problems with Input system events for exemple. 
            //GetGraphics()->Renderer();
        }
    }
}