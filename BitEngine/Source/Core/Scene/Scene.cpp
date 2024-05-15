#include "Core/Scene/Scene.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Graphics/LightManager.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/TestComponent.h"
#include "Components/PointLightComponent.h"

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

   std::vector<Mesh> meshs = 
        Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\HeroGoat.bitMesh");

    std::vector<Mesh> cubeMeshs =
        Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\cube.bitMesh");

    meshComponent->AddMeshs(meshs);
    meshComponent2->AddMeshs(cubeMeshs);
    meshComponent3->AddMeshs(cubeMeshs);

    Material material0;
    material0.SetShader("Simple");
    material0.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1001_Diffuse.png", 0);
    
    Material material1;
    material1.SetShader("Simple");
    material1.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);
    
    Material material2;
    material2.SetShader("Simple");
    material2.SetTexture("Content\\Textures\\HeroGoat\\Ch40_1002_Diffuse.png", 0);
    
    Material material3;
    material3.SetShader("Simple");
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

    sceneObject2->AddComponent(meshComponent2);
    sceneObject2->AddComponent(materialComponent2);

    sceneObject2->SetScale(Vector3D(100, 1, 100));

    TestComponent* test = new TestComponent();
    PointLightComponent* pointLight = new PointLightComponent();
    sceneObject3->AddComponent(meshComponent3);
    sceneObject3->AddComponent(pointLight);
    sceneObject3->AddComponent(test);
    sceneObject3->AddComponent(materialComponent2);
    pointLight->SetColor(1, 1, 1);
    pointLight->SetStrength(1);

    
    scene->AddObject(sceneObject);
    scene->AddObject(sceneObject2);
    scene->AddObject(sceneObject3);
    scene->AddObject(camera);

    Graphics::Light::LightManager::GetInstance()->SetAmbientLightColor(RColorRGB(1, 1, 1));
    Graphics::Light::LightManager::GetInstance()->SetAmbientLightStrength(1.0f);

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
