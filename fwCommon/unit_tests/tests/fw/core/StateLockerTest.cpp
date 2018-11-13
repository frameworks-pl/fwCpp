#include "gtest.h"
#include "etcore.h"

TEST(StateLockerTest, stateLockerTest) {

    bool bVar = true;
    {
        et::core::StateLocker sl(&bVar, false);
        ASSERT_FALSE(bVar);
    }
    ASSERT_TRUE(bVar);

    {
        et::core::StateLocker sl(&bVar, false);
        ASSERT_TRUE(bVar);
    }

    ASSERT_TRUE(bVar);

}