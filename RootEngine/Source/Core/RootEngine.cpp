#include "Core/RootEngine.h"
#include "Data/RVector3D.h"
#include "timeapi.h"
#include "FaiaInputSystem.h"

namespace Faia
{
    namespace Root
    {
        using namespace Faia::InputSystem;

        RootEngine::RootEngine(HWND windowHandler) : _windowHandler(windowHandler)
        {}

        void RootEngine::Init()
        {
            _lightManager = std::make_unique<Graphics::Light::LightManager>();

            _previousTime = timeGetTime();
            _graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
            _graphicsMain->SetupDevice();

            _sceneManager = std::make_unique<SceneManager>();
            _sceneManager->LoadScene(Scene::MakeDemoScene());
            _sceneManager->Init();

        }


        void RootEngine::Update()
        {
            DWORD currentTime = timeGetTime();
            float deltaTime = (currentTime - _previousTime) / 1000.0f;
            _previousTime = currentTime;

            _sceneManager->Update(deltaTime);
            //TODO: I need to make the render a separeted thread or the vsync will stop my application and i will have some problems with Input system events for exemple. 
            _graphicsMain->Renderer();

            FaiaInputSystem::GetInstance()->Update(deltaTime);
        }
    }
}