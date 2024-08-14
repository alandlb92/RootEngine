#pragma once
#include "pch.h"
#include "Data/RMatrix4x4.h"

TEST(RMatrix4X4, CTor) 
{
    Faia::Root::RMatrix4x4 A;
    Faia::Root::RMatrix4x4 B(.0f, .0f, .0f, .0f,
        .0f, .0f, .0f, .0f,
        .0f, .0f, .0f, .0f,
        .0f, .0f, .0f, .0f);

    EXPECT_EQ(A, B);
}

TEST(RMatrix4X4, OpertarotEqual)
{
    Faia::Root::RMatrix4x4 A(1.0f, .0f, .0f, .0f,
        .0f, 2.0f, .0f, .03f,
        .01f, .0f, .0f, 3.0f,
        .01f, .0f, .0f, 4.0f);

    EXPECT_TRUE(A == A);
}

TEST(RMatrix4X4, CreateIdentity)
{
    Faia::Root::RMatrix4x4 A(1.0f, .0f,   .0f,  .0f,
                              .0f, 1.0f,  .0f,  .0f,
                              .0f,  .0f, 1.0f,  .0f,
                              .0f,  .0f,  .0f,  1.0f);
    Faia::Root::RMatrix4x4 B = Faia::Root::RMatrix4x4::Identity();

    EXPECT_EQ(A, B);
}

TEST(RMatrix4X4, Transpose)
{
    Faia::Root::RMatrix4x4 A( 1.0f,  2.0f,  3.0f,  4.0f,
                              5.0f,  6.0f,  7.0f,  8.0f,
                              9.0f,  10.0f, 11.0f, 12.0f,
                              13.0f, 14.0f, 15.0f, 16.0f);

    Faia::Root::RMatrix4x4 B = A.Transpose();

    Faia::Root::RMatrix4x4 expectedResult(1.0f, 5.0f, 9.0f, 13.0f,
                                          2.0f, 6.0f, 10.0f, 14.0f,
                                          3.0f, 7.0f, 11.0f, 15.0f,
                                          4.0f, 8.0f, 12.0f, 16.0f);

    EXPECT_EQ(B, expectedResult);
}

TEST(RMatrix4X4, QuatMultQuat)
{
    Faia::Root::RMatrix4x4 A(1.0f, 2.0f, 3.0f, 4.0f,
                             5.0f, 6.0f, 7.0f, 8.0f,
                             9.0f, 10.0f, 11.0f, 12.0f,
                             13.0f, 14.0f, 15.0f, 16.0f);

    Faia::Root::RMatrix4x4 B(16.0f, 15.0f, 14.0f, 13.0f,
                             12.0f, 11.0f, 10.0f,  9.0f,
                              8.0f,  7.0f,  6.0f,  5.0f,
                              4.0f,  3.0f,  2.0f,  1.0f);

    Faia::Root::RMatrix4x4 C = A * B;
    Faia::Root::RMatrix4x4 expectedResult(   80.f,  70.f,  60.f,  50.f,
                                            240.f, 214.f, 188.f, 162.f,
                                            400.f, 358.f, 316.f, 274.f,
                                            560.f, 502.f, 444.f, 386.f);

    EXPECT_EQ(C, expectedResult);
}

TEST(RMatrix4X4, CreateTranslationMatrix)
{
    Faia::Root::RVector3D position = Faia::Root::RVector3D(15.f, 15.f, 10.f);
    Faia::Root::RMatrix4x4 A = Faia::Root::RMatrix4x4::CreateTransitionMatrix(position);

    Faia::Root::RMatrix4x4 expectedResult(1.f, .0f, .0f, 15.f,
                                         .0f,  1.f, .0f, 15.f,
                                         .0f,  .0f, 1.f, 10.f,
                                         .0f,  .0f, .0f, 1.f);

    EXPECT_EQ(A, expectedResult);
}

TEST(RMatrix4X4, CreateRotationMatrix)
{
    Faia::Root::RQuaternion rotation = Faia::Root::RQuaternion(0.4619398f, 0.1913417f, 0.4619398f, 0.7325378f);
    Faia::Root::RMatrix4x4 A = Faia::Root::RMatrix4x4::CreateRotationMatrixFromQuaternion(rotation);

    

    Faia::Root::RMatrix4x4 expectedResult(0.499999940f      , -0.500000060f      , 0.707106829f,  .0f,
                                          0.853553474f, 0.146446466f, -0.500000060f     ,   .0f,
                                          0.146446705f, 0.853553474f, 0.499999940f     ,   .0f,
                                          .0f      ,  .0f      ,  .0f     ,  1.0f);

    EXPECT_TRUE(A == expectedResult);
}

TEST(RMatrix4X4, CreateScaleMatrix)
{
    Faia::Root::RVector3D scale = Faia::Root::RVector3D(15.f, 15.f, 10.f);
    Faia::Root::RMatrix4x4 A = Faia::Root::RMatrix4x4::CreateScaleMatrix(scale);

    Faia::Root::RMatrix4x4 expectedResult(   15.f,  .0f,  .0f,  .0f,
                                            .0f, 15.f, .0f, .0f,
                                            .0f, .0f, 10.f, .0f,
                                            .0f, .0f, .0f, 1.f);

    EXPECT_EQ(A, expectedResult);
}

TEST(RMatrix4X4, CreateTransformationMatrix)
{
    Faia::Root::RVector3D position = Faia::Root::RVector3D(15.f, 15.f, 10.f);
    Faia::Root::RQuaternion rotation = Faia::Root::RQuaternion(0.4619398f, 0.1913417f, 0.4619398f, 0.7325378f);
    Faia::Root::RVector3D scale = Faia::Root::RVector3D(15.f, 15.f, 10.f);

    Faia::Root::RMatrix4x4 A = Faia::Root::RMatrix4x4::CreateTransformationMatrix(position, rotation, scale);
    Faia::Root::RMatrix4x4 expectedResult(7.49999905f, -7.50000095f, 10.6066027f, 15.0000000f,
                                            12.8033018f, 2.19669700f, -7.50000095f, 15.0000000f,
                                            1.46446705f, 8.53553486f, 4.99999952f, 10.0000000f,
                                            .0f, .0f, .0f, 1.f);

    EXPECT_EQ(A, expectedResult);
}