
#include "pch.h"
#include "Camera.h"
#include "GraphicsMain.h"


//static float angle = 0.0f;
//angle += 30.0f * deltaTime;
//XMVECTOR rotationAxis = XMVectorSet(0.5f, .5f, .5f, 0);
//g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));

Camera::Camera()
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
    _eyePosition = XMVectorSet(0, 100, 400, 1);
    _focusPoint = XMVectorSet(0, 100, 0, 1);
    _upDirection = XMVectorSet(0, 1, 0, 0);
    _viewMatrix = XMMatrixLookAtLH(_eyePosition, _focusPoint, _upDirection);
    _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), XMConvertToRadians(0));
    _fieldOfView = XMConvertToRadians(45.0f); // Ângulo de visão de 45 graus
    float w = GraphicsMain::GetInstace()->GetWidth();
    float h = GraphicsMain::GetInstace()->GetHeight();
    _aspectRatio = w / h; // Proporção de aspecto da tela
    _nearPlane = 0.1f; // Distância do plano de visão próximo
    _farPlane = 1000.0f; // Distância do plano de visão distante
    _projectionMatrix = XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
}

void Camera::Update(float deltaTime)
{
    if (_rotate)
    {
        angle += 30.0f * deltaTime;
        _viewMatrix = XMMatrixLookAtLH(_eyePosition, _focusPoint, _upDirection);
        _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), XMConvertToRadians(angle));
    }

    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Frame, &_viewMatrix);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Object, &_worldMatrix);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Application, &_projectionMatrix);
}

void Camera::StartRotate()
{
    OutputDebugStringA("StartRotate");
    _rotate = true;
}

void Camera::StopRotate()
{
    OutputDebugStringA("StopRotate");
    _rotate = false;
}