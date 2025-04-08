#pragma once
#include "pch.h"
#include "Faia/HashUtils.h"
#include "Data/RMatrix4x4.h"
#include "directxmath.h"

TEST(MathTests, CalculatePorjectionMatrix) 
{
    float fieldOfView;
    float aspectRatio;
    float nearPlane;
    float farPlane;


    Faia::Root::RMatrix4x4 projectionMatrix;
    //RMatrix4x4 _projectionMatrix = XMMatrixPerspectiveFovLH(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
}
