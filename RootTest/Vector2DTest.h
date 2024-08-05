#pragma once
#include "pch.h"
#include "Data/Vector2D.h"

TEST(Vector2D, cTor)
{
    Faia::Root::Vector2D A;
    Faia::Root::Vector2D B(.0f, .0f);
    Faia::Root::Vector2D C(.0f);
    Faia::Root::Vector2D D(1.f, 1.f);
    Faia::Root::Vector2D E(1.f);

    EXPECT_EQ(A, B);
    EXPECT_EQ(A, C);
    EXPECT_EQ(D, E);
}
TEST(Vector2D, EqualOperator) 
{
    Faia::Root::Vector2D A(.5f, .2f);
    Faia::Root::Vector2D B(.2f, .34f);
    Faia::Root::Vector2D C(.52f, .1f);

    EXPECT_TRUE(A == A);
    EXPECT_FALSE(A == B);
    EXPECT_FALSE(C == B);
}

TEST(Vector2D, VecPlusFloatOperator)
{
    float fA = .5f;
    Faia::Root::Vector2D A(.5f, .2f);
    Faia::Root::Vector2D resultExpected(1.f, .7f);
    Faia::Root::Vector2D C = A + fA;

    EXPECT_EQ(C, resultExpected);
}

TEST(Vector2D, VecPlusEqualVecOperator)
{
    Faia::Root::Vector2D A(.5f, .2f);
    Faia::Root::Vector2D B(.2f, .5f);
    Faia::Root::Vector2D resultExpected(.7f, .7f);
    A += B;

    EXPECT_EQ(A, resultExpected);
}

TEST(Vector2D, VecMultFloatOperator)
{
    float fA = .5f;
    Faia::Root::Vector2D A(1.);
    Faia::Root::Vector2D C = A * fA;
    Faia::Root::Vector2D resultExpected(0.5f ,0.5f);

    EXPECT_EQ(C, resultExpected);
}
