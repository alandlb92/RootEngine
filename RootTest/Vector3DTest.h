#pragma once
#include "pch.h"
#include "Data/Vector3D.h"

TEST(Vector3D, cTor)
{
    Faia::Root::Vector3D A;
    Faia::Root::Vector3D B(.0f, .0f, .0f);
    Faia::Root::Vector3D C(.0f);
    Faia::Root::Vector3D D(1.f, 1.f, 1.f);
    Faia::Root::Vector3D E(1.f);

    EXPECT_EQ(A, B);
    EXPECT_EQ(A, C);
    EXPECT_EQ(D, E);
}
TEST(Vector3D, EqualOperator)
{
    Faia::Root::Vector3D A(.5f, .2f, .3f);
    Faia::Root::Vector3D B(.2f, .34f, .55f);
    Faia::Root::Vector3D C(.52f, .1f, .64f);

    EXPECT_TRUE(A == A);
    EXPECT_FALSE(A == B);
    EXPECT_FALSE(C == B);
}

TEST(Vector3D, VecPlusFloatOperator)
{
    float fA = .5f;
    Faia::Root::Vector3D A(.5f, .2f, .3f);
    Faia::Root::Vector3D resultExpected(1.f, .7f, .8f);
    Faia::Root::Vector3D C = A + fA;

    EXPECT_EQ(C, resultExpected);
}

TEST(Vector3D, VecPlusEqualVecOperator)
{
    Faia::Root::Vector3D A(.5f, .2f, .3f);
    Faia::Root::Vector3D B(.2f, .5f, .1f);
    Faia::Root::Vector3D resultExpected(.7f, .7f, .4f);
    A += B;

    EXPECT_EQ(A, resultExpected);
}

TEST(Vector3D, VecPlusVecOperator)
{
    Faia::Root::Vector3D A(.5f, .2f, .3f);
    Faia::Root::Vector3D B(.2f, .5f, .1f);
    Faia::Root::Vector3D resultExpected(.7f, .7f, .4f);
    Faia::Root::Vector3D C = A + B;

    EXPECT_EQ(C, resultExpected);
}

TEST(Vector3D, VecMultFloatOperator)
{
    float fA = .5f;
    Faia::Root::Vector3D A(1.);
    Faia::Root::Vector3D C = A * fA;
    Faia::Root::Vector3D resultExpected(0.5f, 0.5f, .5f);

    EXPECT_EQ(C, resultExpected);
}

TEST(Vector3D, VecMultVecOperator)
{
    Faia::Root::Vector3D A(.5f, 10.f, 15.f);
    Faia::Root::Vector3D B(1.f, 2.f, 3.f);
    Faia::Root::Vector3D C = A * B;
    Faia::Root::Vector3D resultExpected(0.5f, 20.f, 45.f);

    EXPECT_EQ(C, resultExpected);
}