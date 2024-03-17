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
    void Init();

private:
    float angle = 0.0f;
    bool _rotate = false;

    float _fieldOfView;
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;

    XMVECTOR _eyePosition;
    XMVECTOR _focusPoint;
    XMVECTOR _upDirection;
    XMMATRIX _worldMatrix;
    XMMATRIX _viewMatrix;
    XMMATRIX _projectionMatrix;

};