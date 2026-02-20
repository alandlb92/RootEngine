
#include "Graphics/DX11/RCamera_DX11.h"
#include "Graphics/DX11/GraphicsMain_DX11.h"
#include "Graphics/DX11/RConstantBuffersHandler_DX11.h"
#include "FaiaInputSystem.h"
#include "Faia/Converter.h"
#include "Faia/FMath.h"

namespace Faia
{
    namespace Root
    {
        RCamera_DX11::RCamera_DX11() : Super("Camera")
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

            _forward = RVector3D(0.0f, 0.0f, 1.0f);
            _left = RVector3D(-1.0f, 0.0f, 0.0f);
            _right = RVector3D(1.0f, 0.0f, 0.0f);
            _backward = RVector3D(0.0f, 0.0f, -1.0f);

            _position = RVector3D(0, 0, -100);
            _rotation = RVector3D(0);
        }

        using namespace Faia::InputSystem;

        void RCamera_DX11::Init()
        {
            Super::Init();
            UpdateViewMatrix();           
            
            ConfigureProjectionMatrix(500, 500);

            GetFaiaInputSystem()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::W, std::bind(&RCamera_DX11::MoveCameraYFront, this, std::placeholders::_1));
            GetFaiaInputSystem()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::S, std::bind(&RCamera_DX11::MoveCameraYBack, this, std::placeholders::_1));
            GetFaiaInputSystem()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::A, std::bind(&RCamera_DX11::MoveCameraXRight, this, std::placeholders::_1));
            GetFaiaInputSystem()->RegisterActionEvent(InputEventType::KEY_HELD, KeyCode::D, std::bind(&RCamera_DX11::MoveCameraXLeft, this, std::placeholders::_1));

            GetFaiaInputSystem()->RegisterAxisEvent(AxisType::MOUSE_X, std::bind(&RCamera_DX11::RotateCameraX, this, std::placeholders::_1, std::placeholders::_2), { KeyCode::MOUSE_LEFT });
            GetFaiaInputSystem()->RegisterAxisEvent(AxisType::MOUSE_Y, std::bind(&RCamera_DX11::RotateCameraY, this, std::placeholders::_1, std::placeholders::_2), { KeyCode::MOUSE_LEFT });

        }

        void RCamera_DX11::ConfigureProjectionMatrix(float viewWidth, float viewHeight)
        {
            _worldMatrix = XMMatrixRotationAxis(XMVectorSet(.5f, .5f, .5f, .0f), Faia::Math::AngleToRadians(0));
            _fieldOfView =  Faia::Math::AngleToRadians(45.0f); // Ângulo de visão de 45 graus
            _aspectRatio = viewWidth / viewHeight;
            _nearPlane = 0.1f;
            _farPlane = 100000.0f;
            _projectionMatrix = XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
            Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gWorldMatrixHash, &_worldMatrix);
            Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gProjectionMatrixHash, &_projectionMatrix);
        }

        void RCamera_DX11::AddLocalPosition(RVector3D positionToAdd)
        {
            _position += (_right * positionToAdd.X);
            _position += (_forward * positionToAdd.Z);
            _position += (positionToAdd.Y);
            UpdateViewMatrix();
        }

        void RCamera_DX11::AddWorldPosition(RVector3D positionToAdd)
        {
            _position += positionToAdd;
            UpdateViewMatrix();
        }

        void RCamera_DX11::Rotate(float pitch, float yaw, float roll)
        {
            _rotation += RVector3D(pitch, yaw, roll);
            UpdateViewMatrix();
        }

        void RCamera_DX11::UpdateViewMatrix()
        {
            //Calculate camera rotation matrix
            XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->_rotation.X, this->_rotation.Y, this->_rotation.Z);

            //Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
            XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
            XMFLOAT3 XMFcamPos = XMFLOAT3(_position.X, _position.Y, _position.Z);
            XMVECTOR XMVcamPosition = XMLoadFloat3(&XMFcamPos);

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
            _forward = RVector3D(float3Foward.x, float3Foward.y, float3Foward.z);
            XMFLOAT3 float3Backward;
            XMStoreFloat3(&float3Backward, vec_backward);
            _backward = RVector3D(float3Backward.x, float3Backward.y, float3Backward.z);
            XMFLOAT3 float3Left;
            XMStoreFloat3(&float3Left, vec_left);
            _left = RVector3D(float3Left.x, float3Left.y, float3Left.z);
            XMFLOAT3 float3Right;
            XMStoreFloat3(&float3Right, vec_right);
            _right = RVector3D(float3Right.x, float3Right.y, float3Right.z);

            //update buffer
            Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gViewMatrixHash, &_viewMatrix);
        }

        void RCamera_DX11::Update(float deltaTime)
        {
            Graphics::GetConstantBuffersHandler()->SetParamData(Graphics::gcameraPositionHash, &_position);
            Super::Update(deltaTime);
        }

        void RCamera_DX11::MoveCameraYFront(float deltaTime)
        {
            AddLocalPosition(RVector3D(0, 0, cameraTranslationVelocity * deltaTime));
        }

        void RCamera_DX11::MoveCameraYBack(float deltaTime)
        {
            AddLocalPosition(RVector3D(0, 0, -cameraTranslationVelocity * deltaTime));
        }

        void RCamera_DX11::MoveCameraXRight(float deltaTime)
        {
            AddLocalPosition(RVector3D(-cameraTranslationVelocity * deltaTime, 0, 0));
        }

        void RCamera_DX11::MoveCameraXLeft(float deltaTime)
        {
            AddLocalPosition(RVector3D(cameraTranslationVelocity * deltaTime, 0, 0));
        }

        void RCamera_DX11::MoveCameraZUp(float deltaTime)
        {
            AddWorldPosition(RVector3D(0, 100 * deltaTime, 0));
        }

        void RCamera_DX11::MoveCameraZDown(float deltaTime)
        {
            AddWorldPosition(RVector3D(0, -100 * deltaTime, 0));
        }

        void RCamera_DX11::RotateCameraX(float axisValue, float deltaTime)
        {
            Rotate(0, axisValue * deltaTime * cameraRotationVelocity, 0);
        }

        void RCamera_DX11::RotateCameraY(float axisValue, float deltaTime)
        {
            Rotate(axisValue * deltaTime * cameraRotationVelocity, 0, 0);
        }
    }
}