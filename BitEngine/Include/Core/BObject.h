#pragma once
#include "Data/Vector3D.h"

class BObject 
{
protected:
    virtual void Update(float deltaTime);
    virtual void Init();

    Vector3D _rotation;
    Vector3D _position;
};