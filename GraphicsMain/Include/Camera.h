#pragma once
#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

class Camera 
{
public:
    Camera();
    void Update(float deltaTime);

private:
    XMVECTOR _eyePosition;
    XMVECTOR _focusPoint;
    XMVECTOR _upDirection;
    XMMATRIX _worldMatrix;
    XMMATRIX _viewMatrix;

};