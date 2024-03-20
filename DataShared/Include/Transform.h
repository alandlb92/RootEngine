#pragma once
#include "Vector3D.h"

class Transform
{
public:
    Transform();
    Vector3D GetPosition() const;
    virtual void SetPosition(Vector3D newPosition);
    virtual void AddPosition(Vector3D addValue);

private:
    Vector3D _position;
};