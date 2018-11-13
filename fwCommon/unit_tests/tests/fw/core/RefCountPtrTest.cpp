#include "gtest.h"
#include <src/fw/core/fwcore.h>



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
    fw::core::RefCountPtr<bool> refCntPtr;
    EXPECT_EQ(0, refCntPtr.GetRefCount());
}

TEST(RefCountPtrTest, afterCreationObjectShouldHaveRefCountEqualOne) {

    fw::core::RefCountPtr<bool> refCntPtr(new bool(false));
    EXPECT_EQ(1, refCntPtr.GetRefCount());

}

TEST(RefCountPtrTest, testDestructorCalledWhenRefCount0) {

    int i = 101;
    {
        fw::core::RefCountPtr<A> rc(new A(&i));
    }
    EXPECT_EQ(100, i);
}

TEST(RefCountPtrTest, testCopyConstructorIncreasesCount)
{
    int i = 10;
    fw::core::RefCountPtr<A> rfCntPtr(new A(&i));
    EXPECT_EQ(1, rfCntPtr.GetRefCount());
    fw::core::RefCountPtr<A> rfCntPtr2(rfCntPtr);
    EXPECT_EQ(2, rfCntPtr.GetRefCount());
    EXPECT_EQ(2, rfCntPtr2.GetRefCount());
    EXPECT_EQ(10, i);
}

TEST(RefCountPtrTest, testAssignmentOperatorIncreasesCount)
{
    int i = 4;
    fw::core::RefCountPtr<A> rfCntPtr(new A(&i));
    fw::core::RefCountPtr<A> rfCntPtr2 = rfCntPtr;
    EXPECT_EQ(2, rfCntPtr.GetRefCount());
    EXPECT_EQ(2, rfCntPtr2.GetRefCount());
    EXPECT_EQ(4, i);
}

int gI = 1000;
fw::core::RefCountPtr<A> GetAsRefCntPtr()
{    
    fw::core::RefCountPtr<A> rfCntPtr(new A(&gI));
    return rfCntPtr;
}

TEST(RefCountPtrTest, testReturnRefCntPtrFromFunction)
{    
    fw::core::RefCountPtr<A> refCntPtr;
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
        std::vector<fw::core::RefCountPtr<A> > refCntPtVect1;
        {
            std::vector<fw::core::RefCountPtr<A> > refCntPtVect;
            refCntPtVect.push_back(fw::core::RefCountPtr<A>(new A(&a)));
            EXPECT_EQ(1, refCntPtVect.begin()->GetRefCount());
            refCntPtVect1 = refCntPtVect;
            EXPECT_EQ(2, refCntPtVect1.begin()->GetRefCount());
            EXPECT_EQ(9, a);
        }
        EXPECT_EQ(1, refCntPtVect1.begin()->GetRefCount());
    }
    EXPECT_EQ(8, a);

}

