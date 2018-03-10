
#include "gtest.h"
#include <src/fw/core/fwcore.h>

class TestObj
{	

	public:
		TestObj() { m_iOrder = -1; }
		TestObj(int iOrder) { m_iOrder = iOrder;  }
		int m_iOrder;
};

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

TEST(NullableVarTest, objectReplecement)
{
	TestObj testObj1(10);
	fw::core::NullableVar<TestObj> nv = testObj1;
	EXPECT_EQ(10, nv().m_iOrder);

	nv = TestObj(20);
	EXPECT_EQ(20, nv().m_iOrder);
}