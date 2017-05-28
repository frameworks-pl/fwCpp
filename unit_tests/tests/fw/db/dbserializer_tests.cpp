#include "gtest.h"

#include <list>
#include <fwxmlnode.h>
#include <src/main/fwCommon.h>

#include <src/fw/core/fwexception.h>
#include <src/fw/core/fwgenericnotifier.h>
#include <src/fw/core/fwbytebuffer.h>
#include "dbpi.h"

class TestObject : public fw::db::SQLObject
{
	public:
		TestObject(const CString& sName) : m_sName(sName) { }
		bool initialize() { return true; }
		CString getTableName() { return _T("test"); }
	protected:
		CString m_sName;

};


class TestDataSet : public fw::db::SQLObjectDataSet
{
	public:
		TestDataSet() : fw::db::SQLObjectDataSet(fw::db::SQLObjectDataSet(RUNTIME_CLASS(TestObject))) { }
};


TEST(SQLObjectDataSet, offsetIterator) 
{

	fw::db::MaxIDDataSet mMaxIDsSet;

	TestDataSet ds;
	ds.initialize(&mMaxIDsSet);
	for (int i = 0; i < 10; i++)
	{
		CString s;
		s.Format(_T("Object %d"), i);
		TestObject o(s);
		ds.set(&o);
	}

	
	ds.initIterator();
	//const TestObject* pObj = dynamic_cast<const TestObject*>(ds.getNextObject());
	//const TestObject* = dynamic_cast<const TestObject*>(ds.getObjectByOffset(5));
    //EXPECT_EQ(1, refCntPtr.GetRefCount());
	ASSERT(true);

}