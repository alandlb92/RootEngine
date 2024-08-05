#pragma once
#include "Data/Quaternion.h"
#include <sstream>

//Todo: found another place to this define
#define MAX_NUM_OF_ANIMATION_CHANNELS 100

namespace Faia
{
    namespace Root
    {
        struct RMatrix4x4
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;

            RMatrix4x4()
            {
                m00 = 0.0f; m01 = 0.0f; m02 = 0.0f; m03 = 0.0f;
                m10 = 0.0f; m11 = 0.0f; m12 = 0.0f; m13 = 0.0f;
                m20 = 0.0f; m21 = 0.0f; m22 = 0.0f; m23 = 0.0f;
                m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 0.0f;
            };

            RMatrix4x4(float _m00, float _m01, float _m02, float _m03,
                float _m10, float _m11, float _m12, float _m13,
                float _m20, float _m21, float _m22, float _m23,
                float _m30, float _m31, float _m32, float _m33) :
                m00(_m00), m01(_m01), m02(_m02), m03(_m03),
                m10(_m10), m11(_m11), m12(_m12), m13(_m13),
                m20(_m20), m21(_m21), m22(_m22), m23(_m23),
                m30(_m30), m31(_m31), m32(_m32), m33(_m33)
            {

            };

            bool operator==(const RMatrix4x4& value) const
            {
                return  m00 == value.m00 && m01 == value.m01 && m02 == value.m02 && m03 == value.m03 &&
                        m10 == value.m10 && m11 == value.m11 && m12 == value.m12 && m13 == value.m13 &&
                        m20 == value.m20 && m21 == value.m21 && m22 == value.m22 && m23 == value.m23 &&
                        m30 == value.m30 && m31 == value.m31 && m32 == value.m32 && m33 == value.m33;
            }

            RMatrix4x4 Transpose()
            {
                return RMatrix4x4(m00, m10, m20, m30,
                    m01, m11, m21, m31,
                    m02, m12, m22, m32,
                    m03, m13, m23, m33);
            }

            std::string ToPrintableMatrix()
            {
                std::stringstream ss;

                ss << m00 << " " << m01 << " " << m02 << " " << m03 << "\n"
                    << m10 << " " << m11 << " " << m12 << " " << m13 << "\n"
                    << m20 << " " << m21 << " " << m22 << " " << m23 << "\n"
                    << m30 << " " << m31 << " " << m32 << " " << m33 << "\n";


                return ss.str();
            }

            RMatrix4x4 operator* (const RMatrix4x4& value)
            {
                return RMatrix4x4(m00 * value.m00 + m01 * value.m10 + m02 * value.m20 + m03 * value.m30,
                    m00 * value.m01 + m01 * value.m11 + m02 * value.m21 + m03 * value.m31,
                    m00 * value.m02 + m01 * value.m12 + m02 * value.m22 + m03 * value.m32,
                    m00 * value.m03 + m01 * value.m13 + m02 * value.m23 + m03 * value.m33,

                    m10 * value.m00 + m11 * value.m10 + m12 * value.m20 + m13 * value.m30,
                    m10 * value.m01 + m11 * value.m11 + m12 * value.m21 + m13 * value.m31,
                    m10 * value.m02 + m11 * value.m12 + m12 * value.m22 + m13 * value.m32,
                    m10 * value.m03 + m11 * value.m13 + m12 * value.m23 + m13 * value.m33,

                    m20 * value.m00 + m21 * value.m10 + m22 * value.m20 + m23 * value.m30,
                    m20 * value.m01 + m21 * value.m11 + m22 * value.m21 + m23 * value.m31,
                    m20 * value.m02 + m21 * value.m12 + m22 * value.m22 + m23 * value.m32,
                    m20 * value.m03 + m21 * value.m13 + m22 * value.m23 + m23 * value.m33,

                    m30 * value.m00 + m31 * value.m10 + m32 * value.m20 + m33 * value.m30,
                    m30 * value.m01 + m31 * value.m11 + m32 * value.m21 + m33 * value.m31,
                    m30 * value.m02 + m31 * value.m12 + m32 * value.m22 + m33 * value.m32,
                    m30 * value.m03 + m31 * value.m13 + m32 * value.m23 + m33 * value.m33);
            }

            static RMatrix4x4 Identity()
            {
                RMatrix4x4 m;

                m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
                m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = 0.0f;
                m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = 0.0f;
                m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;

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

            static RMatrix4x4 CreateTransformationMatrix(Vector3D& position, Quaternion& quatRotation, Vector3D& scale) {
                float scaleFactor = 1.0f;
                RMatrix4x4 translationMatrix(1, 0, 0, position.X,
                    0, 1, 0, position.Y,
                    0, 0, 1, position.Z,
                    0, 0, 0, 1);

                RMatrix4x4 rotationMatrix = RMatrix4x4::CreateRotationMatrixFromQuaternion(quatRotation);

                RMatrix4x4 scaleMatrix(scale.X, 0, 0, 0,
                    0, scale.Y, 0, 0,
                    0, 0, scale.Z, 0,
                    0, 0, 0, 1);


                //Faia::Debug::Log("translation");
                //Faia::Debug::Log(translationMatrix.ToPrintableMatrix().c_str());

                //Faia::Debug::Log("rotation");
                //Faia::Debug::Log(rotationMatrix.ToPrintableMatrix().c_str());
                //
                //Faia::Debug::Log("scale");
                //Faia::Debug::Log(scaleMatrix.ToPrintableMatrix().c_str());


                RMatrix4x4 sclPRot = scaleMatrix * rotationMatrix;
                //Faia::Debug::Log("sclPRot");
                //Faia::Debug::Log(sclPRot.ToPrintableMatrix().c_str());


                RMatrix4x4 transformationMatrix = sclPRot;
                transformationMatrix.m03 = translationMatrix.m03;
                transformationMatrix.m13 = translationMatrix.m13;
                transformationMatrix.m23 = translationMatrix.m23;

                /*  Faia::Debug::Log("transformationMatrix");
                  Faia::Debug::Log(transformationMatrix.ToPrintableMatrix().c_str());*/
                return transformationMatrix;
            }
        };
    }
}