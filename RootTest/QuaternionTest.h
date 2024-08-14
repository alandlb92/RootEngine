#pragma once
#include "pch.h"
#include "Data/RQuaternion.h"

TEST(Quaternion, cTor)
{
    Faia::Root::RQuaternion A;
    Faia::Root::RQuaternion B(.0f, .0f, .0f, 1.0f);
    Faia::Root::RQuaternion C(1.0f);
    Faia::Root::RQuaternion D(1.f, 1.f, 1.f, 1.f);
    Faia::Root::RQuaternion E(1.f);

    EXPECT_EQ(A, B);
    EXPECT_EQ(D, C);
    EXPECT_EQ(D, E);
}
TEST(Quaternion, EqualOperator)
{
    Faia::Root::RQuaternion A(.5f, .2f, .3f, .5f);
    Faia::Root::RQuaternion B(.2f, .34f, .1f, .8f);
    Faia::Root::RQuaternion C(.52f, .1f, .9f, .054f);

    EXPECT_TRUE(A == A);
    EXPECT_FALSE(A == B);
    EXPECT_FALSE(C == B);
}

TEST(Quaternion, QuatPlusFloatOperator)
{
    float fA = .5f;
    Faia::Root::RQuaternion A(.5f, .2f, .3f, .5f);
    Faia::Root::RQuaternion resultExpected(1.f, .7f, .8f, 1.f);
    Faia::Root::RQuaternion C = A + fA;

    EXPECT_EQ(C, resultExpected);
}

TEST(Quaternion, QuatPlusEqualQuatOperator)
{
    Faia::Root::RQuaternion A(.5f, .2f, .3f, .5f);
    Faia::Root::RQuaternion B(.2f, .5f, .1f, .9f);
    Faia::Root::RQuaternion resultExpected(.7f, .7f, .4f, 1.4f);
    A += B;

    EXPECT_EQ(A, resultExpected);
}