#pragma once
#include "pch.h"
#include "Data/RVector2D.h"

TEST(RVector2D, cTor)
{
    Faia::Root::RVector2D A;
    Faia::Root::RVector2D B(.0f, .0f);
    Faia::Root::RVector2D C(.0f);
    Faia::Root::RVector2D D(1.f, 1.f);
    Faia::Root::RVector2D E(1.f);

    EXPECT_EQ(A, B);
    EXPECT_EQ(A, C);
    EXPECT_EQ(D, E);
}
TEST(RVector2D, EqualOperator) 
{
    Faia::Root::RVector2D A(.5f, .2f);
    Faia::Root::RVector2D B(.2f, .34f);
    Faia::Root::RVector2D C(.52f, .1f);

    EXPECT_TRUE(A == A);
    EXPECT_FALSE(A == B);
    EXPECT_FALSE(C == B);
}

TEST(RVector2D, VecPlusFloatOperator)
{
    float fA = .5f;
    Faia::Root::RVector2D A(.5f, .2f);
    Faia::Root::RVector2D resultExpected(1.f, .7f);
    Faia::Root::RVector2D C = A + fA;

    EXPECT_EQ(C, resultExpected);
}

TEST(RVector2D, VecPlusEqualVecOperator)
{
    Faia::Root::RVector2D A(.5f, .2f);
    Faia::Root::RVector2D B(.2f, .5f);
    Faia::Root::RVector2D resultExpected(.7f, .7f);
    A += B;

    EXPECT_EQ(A, resultExpected);
}

TEST(RVector2D, VecMultFloatOperator)
{
    float fA = .5f;
    Faia::Root::RVector2D A(1.);
    Faia::Root::RVector2D C = A * fA;
    Faia::Root::RVector2D resultExpected(0.5f ,0.5f);

    EXPECT_EQ(C, resultExpected);
}
