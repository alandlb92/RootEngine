#pragma once
#include "Core/Scene/SceneObject.h"
#include "Data/Quaternion.h"

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
            void AddWorldPosition(Vector3D positionToAdd);
            void AddLocalPosition(Vector3D positionToAdd);
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

            Vector3D _forward;
            Vector3D _left;
            Vector3D _right;
            Vector3D _backward;

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