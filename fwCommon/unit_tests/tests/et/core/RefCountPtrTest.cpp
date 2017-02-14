
#include "gtest.h"
#include <src/et/core/etcore.h>

#define _USING_V110_SDK71_

class A
{
    public:
        A(int* i) : m_pInt(i) { }
        virtual ~A() {
            *m_pInt -= 1;
        }
    protected:
        int* m_pInt;
};

TEST(RefCountPtrTest, emptyRefCountPtrShouldHaveCountZero)
{
    et::core::RefCountPtr<bool> refCntPtr;
    EXPECT_EQ(0, refCntPtr.GetRefCount());
}

TEST(RefCountPtrTest, afterCreationObjectShouldHaveRefCountEqualOne) {

    et::core::RefCountPtr<bool> refCntPtr(new bool(false));
    EXPECT_EQ(1, refCntPtr.GetRefCount());

}

TEST(RefCountPtrTest, testDestructorCalledWhenRefCount0) {

    int i = 101;
    {
        et::core::RefCountPtr<A> rc(new A(&i));
    }
    EXPECT_EQ(100, i);
}

TEST(RefCountPtrTest, testCopyConstructorIncreasesCount)
{
    int i = 10;
    et::core::RefCountPtr<A> rfCntPtr(new A(&i));
    EXPECT_EQ(1, rfCntPtr.GetRefCount());
    et::core::RefCountPtr<A> rfCntPtr2(rfCntPtr);
    EXPECT_EQ(2, rfCntPtr.GetRefCount());
    EXPECT_EQ(2, rfCntPtr2.GetRefCount());
    EXPECT_EQ(10, i);
}

TEST(RefCountPtrTest, testAssignmentOperatorIncreasesCount)
{
    int i = 4;
    et::core::RefCountPtr<A> rfCntPtr(new A(&i));
    et::core::RefCountPtr<A> rfCntPtr2 = rfCntPtr;
    EXPECT_EQ(2, rfCntPtr.GetRefCount());
    EXPECT_EQ(2, rfCntPtr2.GetRefCount());
    EXPECT_EQ(4, i);
}

int gI = 1000;
et::core::RefCountPtr<A> GetAsRefCntPtr()
{    
    et::core::RefCountPtr<A> rfCntPtr(new A(&gI));
    return rfCntPtr;
}

TEST(RefCountPtrTest, testReturnRefCntPtrFromFunction)
{    
    et::core::RefCountPtr<A> refCntPtr;
    EXPECT_EQ(0, refCntPtr.GetRefCount());
    {
        refCntPtr = GetAsRefCntPtr();
        EXPECT_EQ(1, refCntPtr.GetRefCount());
    }
    EXPECT_EQ(1, refCntPtr.GetRefCount());
}

TEST(RefCountPtrTest, testCopyStdVectorOfRefCntPtrs)
{
    int a = 9;
    {
        std::vector<et::core::RefCountPtr<A> > refCntPtVect1;
        {
            std::vector<et::core::RefCountPtr<A> > refCntPtVect;
            refCntPtVect.push_back(et::core::RefCountPtr<A>(new A(&a)));
            EXPECT_EQ(1, refCntPtVect.begin()->GetRefCount());
            refCntPtVect1 = refCntPtVect;
            EXPECT_EQ(2, refCntPtVect1.begin()->GetRefCount());
            EXPECT_EQ(9, a);
        }
        EXPECT_EQ(1, refCntPtVect1.begin()->GetRefCount());
    }
    EXPECT_EQ(8, a);

}

