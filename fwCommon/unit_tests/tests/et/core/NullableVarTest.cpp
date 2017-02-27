#include "gtest.h"
#include <src/fw/core/fwcore.h>


TEST(NullableVarTest, dereferencingValueNotSetThrowsException) {

    fw::core::NullableVar<int> nullableInt;
    try
    {
        nullableInt();
        FAIL();        
    }
    catch (std::string e)
    {
        ASSERT_STREQ("Trying to read nullable value that is not set. Use IsSet first.", e.c_str());
    }
    
}

TEST(NullableVarTest, intInitialization) {
    fw::core::NullableVar<int> nullableInt(10);
    EXPECT_EQ(10, nullableInt());
}