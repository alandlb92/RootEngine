
#include "Graphics/Camera.h"
#include "Graphics/GraphicsMain.h"

Camera::Camera()// : Super()
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

    _forward = Vector3D(0.0f, 0.0f, 1.0f);
    _left = Vector3D(-1.0f, 0.0f, 0.0f);
    _right = Vector3D(1.0f, 0.0f, 0.0f);
    _backward = Vector3D(0.0f, 0.0f, -1.0f);

    _position = Vector3D(0,0,-1000);
    _rotation = Vector3D(0);
}

void Camera::Init()
{
    Super::Init();
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

void Camera::AddLocalPosition(Vector3D positionToAdd)
{
    _position += (_right * positionToAdd.X);
    _position += (_forward * positionToAdd.Z);
    _position += (positionToAdd.Y);
    UpdateViewMatrix();
}

void Camera::AddWorldPosition(Vector3D positionToAdd)
{
    _position += positionToAdd;
    UpdateViewMatrix();
}

void Camera::Rotate(float pitch, float yaw, float roll) 
{
    _rotation += Vector3D(pitch, yaw, roll);
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    //Calculate camera rotation matrix
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->_rotation.X, this->_rotation.Y, this->_rotation.Z);

    //Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
    XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    XMFLOAT3 XMFcamPos = XMFLOAT3(_position.X, _position.Y, _position.Z);
    XMVECTOR XMVcamPosition =  XMLoadFloat3(&XMFcamPos);

    //Adjust cam target to be offset by the camera's current position
    camTarget += XMVcamPosition;

    //Calculate up direction based on current rotation
    XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
    
    //Rebuild view matrix
    _viewMatrix = XMMatrixLookAtLH(XMVcamPosition, camTarget, upDir);

	XMMATRIX vecRotationMatrixY = XMMatrixRotationRollPitchYaw(_rotation.X, _rotation.Y, 0.0f);

    //Conversions to storage data about vectors 
    XMVECTOR vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixY);
    XMVECTOR vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrixY);
    XMVECTOR vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixY);
    XMVECTOR vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrixY);
    
    XMFLOAT3 float3Foward;
    
    XMStoreFloat3(&float3Foward, vec_forward);
    _forward = Vector3D(float3Foward.x, float3Foward.y, float3Foward.z);
    XMFLOAT3 float3Backward;
    XMStoreFloat3(&float3Backward, vec_backward);
    _backward = Vector3D(float3Backward.x, float3Backward.y, float3Backward.z);
    XMFLOAT3 float3Left;
    XMStoreFloat3(&float3Left, vec_left);
    _left = Vector3D(float3Left.x, float3Left.y, float3Left.z);
    XMFLOAT3 float3Right;
    XMStoreFloat3(&float3Right, vec_right);
    _right = Vector3D(float3Right.x, float3Right.y, float3Right.z);

    OutputDebugStringA(_forward.ToString().c_str());

    //update buffer
    GraphicsMain::UpdateConstantBuffer(ConstantBuffer::CB_Frame, &_viewMatrix);
}

void Camera::Update(float deltaTime)
{
    Super::Update(deltaTime);
}