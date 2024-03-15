#pragma once
#include "d3d11.h"
#include "directxmath.h"

using namespace DirectX;

class Camera 
{
public:
    Camera();
    void Update(float deltaTime);
    void StartRotate();
    void StopRotate();

private:
    float angle = 0.0f;
    bool _rotate = false;

    XMVECTOR _eyePosition;
    XMVECTOR _focusPoint;
    XMVECTOR _upDirection;
    XMMATRIX _worldMatrix;
    XMMATRIX _viewMatrix;

};