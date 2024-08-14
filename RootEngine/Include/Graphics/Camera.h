#pragma once
#include "Core/Scene/SceneObject.h"
#include "Data/RQuaternion.h"

#include "d3d11.h"
#include "directxmath.h"

//Todo: remove direcx dependency, found the matrix formula for perpective camera and use RMatrix4x4
using namespace DirectX;

namespace Faia
{
    namespace Root
    {
        class Camera : public SceneObject
        {
            typedef SceneObject Super;

        public:
            Camera();
            void Update(float deltaTime);
            void Init();
            void AddWorldPosition(RVector3D positionToAdd);
            void AddLocalPosition(RVector3D positionToAdd);
            void Rotate(float pitch, float yaw, float roll);
        private:
            void ChangeBonePlusTest();
            void ChangeBoneMinusTest();

            float cameraRotationVelocity = 500.0f;
            float cameraTranslationVelocity = 200.0f;
            void UpdateViewMatrix();

            float _fieldOfView;
            float _aspectRatio;
            float _nearPlane;
            float _farPlane;

            XMVECTOR _eyePosition;
            XMVECTOR _focusPoint;
            XMVECTOR _upDirection;
            XMMATRIX _viewMatrix;
            XMMATRIX _projectionMatrix;

            const float eyeDistance = 100.0f;

            XMMATRIX _worldMatrix;

            RVector3D _forward;
            RVector3D _left;
            RVector3D _right;
            RVector3D _backward;

            const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
            const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
            const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


            void MoveCameraYFront(float deltaTime);
            void MoveCameraYBack(float deltaTime);
            void MoveCameraXRight(float deltaTime);
            void MoveCameraXLeft(float deltaTime);
            void MoveCameraZUp(float deltaTime);
            void MoveCameraZDown(float deltaTime);

            void RotateCameraX(float axisValue, float deltaTime);
            void RotateCameraY(float axisValue, float deltaTime);
        };
    }
}