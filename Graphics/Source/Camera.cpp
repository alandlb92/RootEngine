
#include "pch.h"
#include "Camera.h"
#include "GraphicsMain.h"


//static float angle = 0.0f;
//angle += 30.0f * deltaTime;
//XMVECTOR rotationAxis = XMVectorSet(0.5f, .5f, .5f, 0);
//g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));

Camera::Camera() : Super()
{
    _eyePosition = XMVECTOR();
    _focusPoint = XMVECTOR();
    _upDirection = XMVECTOR();
    _viewMatrix = XMMATRIX();
    _worldMatrix = XMMATRIX();
    _fieldOfView = 0;
    _aspectRatio = 0;
    _nearPlane = 0;
    _farPlane = 0;
}

void Camera::Init()
{
    Super::SetPosition(Vector3D(0, -300, 100));
    UpdateViewMatrix();
    _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), XMConvertToRadians(0));
    _fieldOfView = XMConvertToRadians(45.0f); // Ângulo de visão de 45 graus
    float w = GraphicsMain::GetInstace()->GetWidth();
    float h = GraphicsMain::GetInstace()->GetHeight();
    _aspectRatio = w / h;
    _nearPlane = 0.1f;
    _farPlane = 1000.0f;
    _projectionMatrix = XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Object, &_worldMatrix);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Application, &_projectionMatrix);
}

void Camera::AddPosition(Vector3D positionToAdd)
{
    Super::AddPosition(positionToAdd);
    UpdateViewMatrix();
}
#include "Windows.h"
#include "sstream"
void Camera::UpdateViewMatrix()
{
    _eyePosition = XMVectorSet(
        GetPosition().X,
        100.0f,
        400.0f + GetPosition().Z,
        1
    );
    _focusPoint = XMVectorSet(
        GetPosition().X,
        100,
        100.0f + GetPosition().Z,
        1
    );
    _upDirection = XMVectorSet(0, 1, 0, 0);
    _viewMatrix = XMMatrixLookAtLH(_eyePosition, _focusPoint, _upDirection);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Frame, &_viewMatrix);
}

void Camera::Update(float deltaTime)
{
    /*AddPosition(Vector3D(100 * deltaTime, 0, 0));*/
}