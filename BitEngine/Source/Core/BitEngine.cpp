#include "Core/BitEngine.h"
#include "Data/Vector3D.h"
#include "timeapi.h"
#include "FaiaInputSystem.h"

namespace Faia
{
    namespace Root
    {
        const float BitEngine::targetFramerate = 30.0f;
        const float BitEngine::maxTimeStep = 1.0f / targetFramerate;

        using namespace Faia::InputSystem;

        BitEngine::BitEngine(HWND windowHandler) : _windowHandler(windowHandler)
        {

        }

        void BitEngine::Init()
        {
            _lightManager = std::make_unique<Graphics::Light::LightManager>();

            _previousTime = timeGetTime();
            _graphicsMain = std::make_unique<GraphicsMain>(_windowHandler);
            _graphicsMain->SetupDevice();

            _sceneManager = std::make_unique<SceneManager>();
            _sceneManager->LoadScene(Scene::MakeDemoScene());
            _sceneManager->Init();

        }


        void BitEngine::Update()
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