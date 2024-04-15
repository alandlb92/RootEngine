#pragma once
#include "SceneObject.h"
#include "list"
#include <memory>

class Scene : public BObject
{
public:
    void AddObject(SceneObject obj);

private:
    std::list<SceneObject> _sceneObjects;
};