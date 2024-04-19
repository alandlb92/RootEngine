#include "Core/Scene/Scene.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Camera.h"
#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/TestComponent.h"


Scene* Scene::MakeDemoScene()
{
    Scene* scene = new Scene();

    SceneObject* sceneObject = new SceneObject();
    SceneObject* sceneObject2 = new SceneObject();

    MaterialComponent* materialComponent = new MaterialComponent();
    MaterialComponent* materialComponent2 = new MaterialComponent();
    MeshComponent* meshComponent = new MeshComponent();
    MeshComponent* meshComponent2 = new MeshComponent();

    std::vector<Mesh> meshs = 
        Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\HeroGoat.bitMesh");

    std::vector<Mesh> cubeMeshs =
        Mesh::MakeFromFbxFile("C:\\Users\\alan.bittencourt\\Documents\\Projects\\Personal\\BitEngine\\x64\\Debug\\Content\\Models\\cube.bitMesh");

    meshComponent->AddMeshs(meshs);
    meshComponent2->AddMeshs(cubeMeshs);

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

    sceneObject->AddComponent(meshComponent);
    sceneObject->AddComponent(materialComponent);

    sceneObject2->AddComponent(meshComponent2);
    sceneObject2->AddComponent(materialComponent2);

    sceneObject2->SetScale(Vector3D(100, 1, 100));

    TestComponent* test = new TestComponent();
    sceneObject->AddComponent(test);

    scene->AddObject(sceneObject);
    scene->AddObject(sceneObject2);
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
