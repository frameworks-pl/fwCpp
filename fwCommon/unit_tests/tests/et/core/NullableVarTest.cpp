#include "gtest.h"
#include <src/et/core/etcore.h>

#define _USING_V110_SDK71_

TEST(NullableVarTest, dereferencingValueNotSetThrowsException) {

    et::core::NullableVar<int> nullableInt;
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
    et::core::NullableVar<int> nullableInt(10);
    EXPECT_EQ(10, nullableInt());
}