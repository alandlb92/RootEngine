#include "Core/Scene/Scene.h"
#include "Graphics/Mesh/RMesh.h"
#include "Graphics/Mesh/RSkeletalMesh.h"
#include "Graphics/RMaterial.h"
#include "Graphics/RCamera.h"
#include "Graphics/LightManager.h"
#include "Components/RSkeletalMeshComponent.h"
#include "Components/RMaterialComponent.h"
#include "Components/TestComponent.h"
#include "Components/RPointLightComponent.h"
#include "Components/RAnimationComponent.h"

namespace Faia
{
    namespace Root
    {
        Scene* MakeDemoScene()
        {
            Scene* scene = new Scene();
            

            SceneObject* sceneObject = new SceneObject();
            SceneObject* sceneObject2 = new SceneObject();
            //SceneObject* sceneObject3 = new SceneObject();

            RMaterialComponent* materialComponent = new RMaterialComponent();
            RMaterialComponent* materialComponent2 = new RMaterialComponent();

            RSkeletalMeshComponent* meshComponent = new RSkeletalMeshComponent();
            meshComponent->LoadMesh("Models\\HeroGoat.rmesh");
            RMeshComponent* meshComponent2 = new RMeshComponent();
            meshComponent2->LoadMesh("Models\\cube.rmesh");
         /*   RMeshComponent* meshComponent3 = new RMeshComponent();
            meshComponent3->LoadMesh("Models\\cube.rmesh");*/

            RAnimationComponent* animationComponent = new RAnimationComponent();
            animationComponent->LoadAnimation("Animations\\testAnimation.ranim");
            animationComponent->LoadBoneInfo("Models\\HeroGoat.rboneinfo");

            RMaterial material0;
            material0.SetShader("Standard");
            material0.SetTexture("Textures\\Goat_Diffuse_ch1.rtexture", 0);

            RMaterial material1;
            material1.SetShader("Standard");
            material1.SetTexture("Textures\\Goat_Diffuse_ch2.rtexture", 0);

            RMaterial material2;
            material2.SetShader("Standard");
            material2.SetTexture("Textures\\Goat_Diffuse_ch2.rtexture", 0);

            RMaterial material3;
            material3.SetShader("Standard");
            material3.SetTexture("Textures\\Goat_Diffuse_ch3.rtexture", 0);

            RMaterial cubeMaterial;
            cubeMaterial.SetShader("Standard");


            materialComponent->AddMaterial(material0);
            materialComponent->AddMaterial(material1);
            materialComponent->AddMaterial(material2);
            materialComponent->AddMaterial(material3);

            materialComponent2->AddMaterial(cubeMaterial);

            RCamera* camera = new RCamera();
            camera->SetPosition(RVector3D(0, 200, 500));
            camera->SetRotation(RVector3D(0.367590f, 110, 0));

            sceneObject->AddComponent(meshComponent);
            sceneObject->AddComponent(materialComponent);
            sceneObject->AddComponent(animationComponent);
            sceneObject->SetPosition(RVector3D(0, 0, 0));
            sceneObject->SetRotation(RVector3D(0, 0, 0));

            sceneObject2->AddComponent(meshComponent2);
            sceneObject2->AddComponent(materialComponent2);

            sceneObject2->SetScale(RVector3D(100, 1, 100));

            TestComponent* test = new TestComponent();
            //RPointLightComponent* pointLight = new RPointLightComponent();
            //sceneObject3->AddComponent(meshComponent3);
            //sceneObject3->AddComponent(pointLight);
            //sceneObject3->AddComponent(test);
            //sceneObject3->AddComponent(materialComponent2);
            //pointLight->SetColor(1, .5f, .2f);
            //pointLight->SetStrength(1);

            scene->AddObject(sceneObject);
            scene->AddObject(sceneObject2);
            //scene->AddObject(sceneObject3);
            scene->AddObject(camera);

            return scene;
        }

        std::list<SceneObject*> Scene::GetRenderablebleObjects()
        {
            return _renderablebleObjects;
        }

        void Scene::AddObject(SceneObject* obj)
        {
            obj->RegisterOnNotifyComponentAddedEvent(bind(&Scene::NotifyComponentAdded, this, std::placeholders::_1, std::placeholders::_2));
            _sceneObjects.push_back(obj);
            if (obj->GetComponentOfType<RMeshComponent>() != nullptr || obj->GetComponentOfType<RSkeletalMeshComponent>() != nullptr)
            {
                _renderablebleObjects.push_back(obj);
            }
        }

        void Scene::Init()
        {
            Super::Init();
            for (auto& comp : _components)
            {
                comp->Init();
            }

            for (auto& obj : _sceneObjects)
            {
                obj->Init();
            }
        }

        void Scene::Update(float deltaTime)
        {
            Super::Update(deltaTime);

            for (auto& comp : _components)
            {
                comp->Update(deltaTime);
            }

            for (auto& obj : _sceneObjects)
            {
                obj->Update(deltaTime);
            }
        }

        void Scene::NotifyComponentAdded(RObject* obj, RComponent* component)
        {
            if (typeid(*component) == typeid(RMeshComponent) || typeid(*component) == typeid(RSkeletalMeshComponent))
            {
                _renderablebleObjects.push_back(dynamic_cast<SceneObject*>(obj));
            }
        }
    }
}