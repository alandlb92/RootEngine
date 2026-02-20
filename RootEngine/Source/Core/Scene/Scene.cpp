#include "Core/Scene/Scene.h"
#include "Graphics/DX11/Mesh/RMesh_Dx11.h"
#include "Graphics/DX11/Mesh/RSkeletalMesh_DX11.h"
#include "Graphics/DX11/RMaterial_DX11.h"
#include "Graphics/DX11/RCamera_DX11.h"
#include "Graphics/DX11/LightManager_DX11.h"
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

            /*SceneObject* sceneObject = new SceneObject("Hero");
            SceneObject* sceneObject2 = new SceneObject("Floor");
            SceneObject* sceneObject3 = new SceneObject("Arrow");
            SceneObject* sceneObject4 = new SceneObject("Sphere");

            RMaterialComponent* materialComponent = new RMaterialComponent();
            RMaterialComponent* materialComponent2 = new RMaterialComponent();
            
            RSkeletalMeshComponent* meshComponent = new RSkeletalMeshComponent();
            meshComponent->LoadMesh("Models\\HeroGoat.rmesh");
            RMeshComponent* meshComponent2 = new RMeshComponent();
            meshComponent2->LoadMesh("Models\\cube.rmesh");
            RMeshComponent* meshComponent3 = new RMeshComponent();
            meshComponent3->LoadMesh("Models\\GizmoArrow.rmesh");
            RMeshComponent* meshComponent4 = new RMeshComponent();
            meshComponent4->LoadMesh("Models\\Sphere.rmesh");

            RAnimationComponent* animationComponent = new RAnimationComponent();
            animationComponent->LoadAnimation("Animations\\testAnimation.ranim");
            animationComponent->LoadBoneInfo("Models\\HeroGoat.rboneinfo");*/

            //Todo Layered graphics: Materials must be generic
          /*  RMaterial_DX11 material0;
            material0.SetShader("Standard");
            material0.SetTexture("Textures\\Goat_Diffuse_ch1.rtexture", 0);

            RMaterial_DX11 material1;
            material1.SetShader("Standard");
            material1.SetTexture("Textures\\Goat_Diffuse_ch2.rtexture", 0);

            RMaterial_DX11 material2;
            material2.SetShader("Standard");
            material2.SetTexture("Textures\\Goat_Diffuse_ch2.rtexture", 0);

            RMaterial_DX11 material3;
            material3.SetShader("Standard");
            material3.SetTexture("Textures\\Goat_Diffuse_ch3.rtexture", 0);

            RMaterial_DX11 cubeMaterial;
            cubeMaterial.SetShader("Standard");*/


            //materialComponent->AddMaterial(material0);
            //materialComponent->AddMaterial(material1);
            //materialComponent->AddMaterial(material2);
            //materialComponent->AddMaterial(material3);

            //materialComponent2->AddMaterial(cubeMaterial);

            /*RCamera_DX11* camera = new RCamera_DX11();
            camera->SetPosition(RVector3D(0, 200, 500));
            camera->SetRotation(RVector3D(0.367590f, 110, 0));

            sceneObject->AddComponent(meshComponent);
            sceneObject->AddComponent(materialComponent);
            sceneObject->AddComponent(animationComponent);
            sceneObject->SetPosition(RVector3D(0, 0, 0));
            sceneObject->SetRotation(RVector3D(0, 0, 0));

            sceneObject2->AddComponent(meshComponent2);
            sceneObject2->AddComponent(materialComponent2);

            sceneObject2->SetScale(RVector3D(1000, 1, 1000));

            TestComponent* test = new TestComponent();
            RDirectionalLightComponent* dirLightComp = new RDirectionalLightComponent();
            sceneObject3->AddComponent(meshComponent3);
            sceneObject3->AddComponent(dirLightComp);
            sceneObject3->AddComponent(test);
            sceneObject3->AddComponent(materialComponent2);

            sceneObject3->SetPosition(RVector3D(0, 200, 0));
            sceneObject3->SetRotation(RVector3D(0, 0, 0));
            sceneObject3->SetScale(RVector3D(2, 2, 2));

            sceneObject4->AddComponent(meshComponent4);
            sceneObject4->AddComponent(materialComponent2);
            sceneObject4->SetPosition(RVector3D(50, 150, 300));
            sceneObject4->SetScale(RVector3D(20, 20, 20));         

            scene->AddObject(sceneObject);
            scene->AddObject(sceneObject2);
            scene->AddObject(sceneObject3);
            scene->AddObject(sceneObject4);

            scene->AddObject(camera);*/

            return scene;
        }

        std::list<SceneObject*> Scene::GetRenderablebleObjects()
        {
            return _renderablebleObjects;
        }

        std::list<SceneObject*> Scene::GetSceneObjects()
        {
            return _sceneObjects;
        }

        void Scene::AddObject(SceneObject* obj)
        {
            obj->RegisterOnNotifyComponentAddedEvent(bind(&Scene::NotifyComponentAdded, this, std::placeholders::_1, std::placeholders::_2));
            _sceneObjects.push_back(obj);
            if (obj->GetComponentOfType<RMeshComponent>() != nullptr || obj->GetComponentOfType<RSkeletalMeshComponent>() != nullptr)
            {
                _renderablebleObjects.push_back(obj);
            }

            if (dynamic_cast<RCamera_DX11*>(obj) != nullptr && mMainCamera == nullptr)
            {
                mMainCamera = static_cast<RCamera_DX11*>(obj);
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