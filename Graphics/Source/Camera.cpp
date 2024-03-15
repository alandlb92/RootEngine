
#include "pch.h"
#include "Camera.h"
#include "GraphicsMain.h"


//static float angle = 0.0f;
//angle += 30.0f * deltaTime;
//XMVECTOR rotationAxis = XMVectorSet(0.5f, .5f, .5f, 0);
//g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));

Camera::Camera()
{
    _eyePosition = XMVectorSet(0, 0, -10, 1);
    _focusPoint = XMVectorSet(0, 0, 0, 1);
    _upDirection = XMVectorSet(0, 1, 0, 0);
    _viewMatrix = XMMatrixLookAtLH(_eyePosition, _focusPoint, _upDirection);
    _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), XMConvertToRadians(0));
}

void Camera::Update(float deltaTime)
{
    OutputDebugStringA("StopRotate");
    if (_rotate)
    {
        angle += 30.0f * deltaTime;
        _viewMatrix = XMMatrixLookAtLH(_eyePosition, _focusPoint, _upDirection);
        _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), XMConvertToRadians(angle));
    }

    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Frame, &_viewMatrix);
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Object, &_worldMatrix);
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
