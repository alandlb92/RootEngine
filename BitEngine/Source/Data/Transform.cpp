#include "Data/Transform.h"


Transform::Transform()
{
    _position = Vector3D(0);
}

Vector3D Transform::GetPosition() const
{
    return _position;
}

void Transform::SetPosition(Vector3D newPosition)
{
    _position = newPosition;
}

void Transform::AddPosition(Vector3D addValue)
{
    _position += addValue;
}

