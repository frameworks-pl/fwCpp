#include "gtest.h"

#include <list>
#include <fwxmlnode.h>
#include <src/main/fwCommon.h>

#include <src/fw/core/fwexception.h>
#include <src/fw/core/fwgenericnotifier.h>
#include <src/fw/core/fwbytebuffer.h>
#include <src/fw/core/fwtextconv.h>
#include "dbpi.h"

#include <fwaesencdec.h>

class TestSQLParam : public fw::db::SQLParam
{
	public:
		TestSQLParam(const CString& pColName, CString* pStringValue) : fw::db::SQLParam(pColName, pStringValue) { }
		const CString& getValue() const { return this->m_pStringValue ? *m_pStringValue : _T("ERROR"); }

};

#define BIND_TESTOBJECT_PARAMS addParam(new TestSQLParam(_T("name"), &m_sName));

class TestObject : public fw::db::SQLObject
{
	public:
		TestObject(const CString& sName) : m_sName(sName) 
		{ 
			BIND_TESTOBJECT_PARAMS
		}

		TestObject(const TestObject& pTestObject)
		{
			*this = pTestObject;
		}

		bool initialize() { return true; }
		CString getTableName() { return _T("test"); }


		const TestObject& TestObject::operator=(const TestObject& pObject)
		{
			fw::db::SQLObject::operator=(pObject);
			m_sName = pObject.m_sName;
			BIND_TESTOBJECT_PARAMS

			return *this;

		}

		//expose params for testing
		fw::db::ParamMap& GetParams() { return m_ParamMap; }
	protected:
		CString m_sName;

};


class TestDataSet : public fw::db::SQLObjectDataSet
{
	public:
		TestDataSet() : fw::db::SQLObjectDataSet(fw::db::SQLObjectDataSet(RUNTIME_CLASS(TestObject))) { }
};



TEST(SQLObject, paramsCopyTest)
{
	TestObject ob1("ala");
	TestObject ob2 = ob1;

	ASSERT_EQ(2, ob2.GetParams().size()); //there should be two params: sql_id and name
	fw::db::ParamMap& params = ob2.GetParams();
	ASSERT_EQ(_T("sql_id"), params[_T("sql_id")]->getColumnName());	
	ASSERT_EQ(_T("name"), params[_T("name")]->getColumnName()); //testing if param is bound properly (points to string member)	
	
	fw::db::SQLParam* pParam = params[_T("name")];
	TestSQLParam* pSQLParam = dynamic_cast<TestSQLParam*>(pParam);
	CString value = pSQLParam->getValue();
	ASSERT_EQ(_T("ala"), value);

}