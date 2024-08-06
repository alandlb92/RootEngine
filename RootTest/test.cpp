#include "pch.h"
#include "Faia/Debug.h"
#include "FaiaUtilsTest.h"
#include "ImporterTests.h"
#include "Vector2DTest.h"
#include "Vector3DTest.h"
#include "QuaternionTest.h"
#include "RMatrix4x4Test.h"

int main(int argc, char** argv) 
{
    if (!ImporterTest::IsTheNeededFilesExists())
    {
        Faia::Debug::PopError("    You need to put the files to test in $(OutputPath)TestContent directory, the files is on drive.\n    If you don't have the TestContent contact alan.dlucas.bittencourt@gmail.com");
        return -1;
    }

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}