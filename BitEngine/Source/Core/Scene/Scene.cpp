#include "Core/Scene/Scene.h"
#include "Graphics/Mesh.h"
#include "Graphics/SkeletalMesh.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Graphics/LightManager.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/TestComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AnimationComponent.h"

namespace Faia
{
    namespace Root
    {
        Scene* Scene::MakeDemoScene()
        {
            Scene* scene = new Scene();

            SceneObject* sceneObject = new SceneObject();
            SceneObject* sceneObject2 = new SceneObject();
            SceneObject* sceneObject3 = new SceneObject();

            MaterialComponent* materialComponent = new MaterialComponent();
            MaterialComponent* materialComponent2 = new MaterialComponent();
            MeshComponent* meshComponent = new MeshComponent();
            MeshComponent* meshComponent2 = new MeshComponent();
            MeshComponent* meshComponent3 = new MeshComponent();

            std::vector<std::shared_ptr<Mesh>> meshs =
                SkeletalMesh::MakeSkeletonMeshFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\HeroGoat.rmesh");

            std::vector<std::shared_ptr<Mesh>> cubeMeshs =
                Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\cube.rmesh");

            AnimationComponent* animationComponent = new AnimationComponent();
            animationComponent->SetAnimation("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Animations\\testAnimation.ranim");
            animationComponent->SetBoneInfo("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\HeroGoat.rboneinfo");

            meshComponent->AddMeshs(meshs);
            meshComponent2->AddMeshs(cubeMeshs);
            meshComponent3->AddMeshs(cubeMeshs);

            Material material0;
            material0.SetShader("SimpleSkinned");
            material0.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1001_Diffuse.png", 0);

            Material material1;
            material1.SetShader("SimpleSkinned");
            material1.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);

            Material material2;
            material2.SetShader("SimpleSkinned");
            material2.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);

            Material material3;
            material3.SetShader("SimpleSkinned");
            material3.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1003_Diffuse.png", 0);

            Material cubeMaterial;
            cubeMaterial.SetShader("Simple");


            materialComponent->AddMaterial(material0);
            materialComponent->AddMaterial(material1);
            materialComponent->AddMaterial(material2);
            materialComponent->AddMaterial(material3);

            materialComponent2->AddMaterial(cubeMaterial);

            Camera* camera = new Camera();
            camera->SetPosition(Vector3D(0, 200, 500));
            camera->SetRotation(Vector3D(0.367590f, 110, 0));

            sceneObject->AddComponent(meshComponent);
            sceneObject->AddComponent(materialComponent);
            sceneObject->AddComponent(animationComponent);
            sceneObject->SetPosition(Vector3D(0, 0, 0));

            sceneObject2->AddComponent(meshComponent2);
            sceneObject2->AddComponent(materialComponent2);

            sceneObject2->SetScale(Vector3D(100, 1, 100));

            TestComponent* test = new TestComponent();
            PointLightComponent* pointLight = new PointLightComponent();
            sceneObject3->AddComponent(meshComponent3);
            sceneObject3->AddComponent(pointLight);
            sceneObject3->AddComponent(test);
            sceneObject3->AddComponent(materialComponent2);
            pointLight->SetColor(1, .5f, .2f);
            pointLight->SetStrength(1);

            scene->AddObject(sceneObject);
            scene->AddObject(sceneObject2);
            scene->AddObject(sceneObject3);
            scene->AddObject(camera);

            Graphics::Light::LightManager::GetInstance()->SetAmbientLightColor(RColorRGB(1, 1, 1));
            Graphics::Light::LightManager::GetInstance()->SetAmbientLightStrength(.2f);

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
            if (obj->GetComponentOfType<MeshComponent>() != nullptr)
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

        void Scene::NotifyComponentAdded(BObject* obj, BComponent* component)
        {
            if (typeid(*component) == typeid(MeshComponent))
            {
                _renderablebleObjects.push_back(dynamic_cast<SceneObject*>(obj));
            }
        }
    }
}