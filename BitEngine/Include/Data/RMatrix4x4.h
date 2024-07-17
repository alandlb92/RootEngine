#pragma once
#include "Data/Quaternion.h"

//Todo: found another place to this define
#define MAX_NUM_OF_ANIMATION_CHANNELS 100
struct RMatrix4x4
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;

    static RMatrix4x4 Identity()
    {
        RMatrix4x4 m;

        m.m00 = 1.0f;m.m01 = 0.0f;m.m02 = 0.0f;m.m03 = 0.0f;       
        m.m10 = 0.0f;m.m11 = 1.0f;m.m12 = 0.0f;m.m13 = 0.0f;
        m.m20 = 0.0f;m.m21 = 0.0f;m.m22 = 1.0f;m.m23 = 0.0f;        
        m.m30 = 0.0f;m.m31 = 0.0f;m.m32 = 0.0f;m.m33 = 1.0f;

        return m;
    }

    static RMatrix4x4 CreateRotationMatrixFromQuaternion(Quaternion& quaternion)
    {
        RMatrix4x4 m;

        float xx = quaternion.X * quaternion.X;
        float yy = quaternion.Y * quaternion.Y;
        float zz = quaternion.Z * quaternion.Z;
        float xy = quaternion.X * quaternion.Y;
        float xz = quaternion.X * quaternion.Z;
        float yz = quaternion.Y * quaternion.Z;
        float wx = quaternion.W * quaternion.X;
        float wy = quaternion.W * quaternion.Y;
        float wz = quaternion.W * quaternion.Z;

        m.m00 = 1.0f - 2.0f * (yy + zz);
        m.m01 = 2.0f * (xy - wz);
        m.m02 = 2.0f * (xz + wy);
        m.m03 = 0.0f;

        m.m10 = 2.0f * (xy + wz);
        m.m11 = 1.0f - 2.0f * (xx + zz);
        m.m12 = 2.0f * (yz - wx);
        m.m13 = 0.0f;

        m.m20 = 2.0f * (xz - wy);
        m.m21 = 2.0f * (yz + wx);
        m.m22 = 1.0f - 2.0f * (xx + yy);
        m.m23 = 0.0f;

        m.m30 = 0.0f;
        m.m31 = 0.0f;
        m.m32 = 0.0f;
        m.m33 = 1.0f;

        return m;
    }

    static RMatrix4x4 CreateTransformationMatrix(Vector3D& scale, Quaternion& quaternion, Vector3D& position) {
        RMatrix4x4 rotationMatrix = RMatrix4x4::CreateRotationMatrixFromQuaternion(quaternion);

        RMatrix4x4 transformationMatrix;

        // Incorpora a escala
        transformationMatrix.m00 = scale.X * rotationMatrix.m00;
        transformationMatrix.m01 = scale.X * rotationMatrix.m01;
        transformationMatrix.m02 = scale.X * rotationMatrix.m02;
        transformationMatrix.m03 = 0.0f;

        transformationMatrix.m10 = scale.Y * rotationMatrix.m10;
        transformationMatrix.m11 = scale.Y * rotationMatrix.m11;
        transformationMatrix.m12 = scale.Y * rotationMatrix.m12;
        transformationMatrix.m13 = 0.0f;

        transformationMatrix.m20 = scale.Z * rotationMatrix.m20;
        transformationMatrix.m21 = scale.Z * rotationMatrix.m21;
        transformationMatrix.m22 = scale.Z * rotationMatrix.m22;
        transformationMatrix.m23 = 0.0f;

        // Incorpora a posição
        transformationMatrix.m30 = position.X;
        transformationMatrix.m31 = position.Y;
        transformationMatrix.m32 = position.Z;
        transformationMatrix.m33 = 1.0f;

        return transformationMatrix;
    }
};