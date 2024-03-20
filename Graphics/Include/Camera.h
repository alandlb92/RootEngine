#pragma once
#include "d3d11.h"
#include "directxmath.h"
#include "Transform.h"

using namespace DirectX;

class Camera : public Transform
{
    typedef Transform Super;

public:
    Camera();
    void Update(float deltaTime);
    void Init();
    void AddPosition(Vector3D positionToAdd) override;

private:
    void UpdateViewMatrix();

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

    const float eyeDistance = 100.0f;

};