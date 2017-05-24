#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{


		MaxIDDataSet::MaxIDDataSet()
			: SQLObjectDataSet(RUNTIME_CLASS(MaxID))
		{

			m_iMaxID = INVALID_SQL_ID;


		}

		MaxID* MaxIDDataSet::findMaxObject(const CString& pObjectName)
		{

			SQLObjectMap::iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
			{
				MaxID* pMaxID = (MaxID*)it->second;
				if (pMaxID && (pMaxID->getObjectName() == pObjectName))
				{
					return pMaxID;
				}
			}

			return NULL;

		}


		SQL_ID MaxIDDataSet::set(const SQLObject* pSQLObject)
		{

			if (SQLObject::STATE_OK_LOADED != pSQLObject->getState())
			{
				//verify that the name of the object being added is unique!!!
				const MaxID* pMaxID = (MaxID*)pSQLObject;
				if (pMaxID)
				{
					MaxID* pExistingMaxID = findMaxObject(pMaxID->getObjectName());
					if (NULL != pExistingMaxID)
					{
						CString s;
						s.Format(_T("Object named '%s' already exists in the max id collection."), pMaxID->getObjectName());
						throw DBException(s);
					}
				}
				else
					throw db::DBException(_T("The provided object is not valid for this collection."));
			}

			SQL_ID sqlId = SQLObjectDataSet::set(pSQLObject);

			//make sure the max id is never less than id of any object added to the set
			if (m_iMaxID < sqlId)
				m_iMaxID = sqlId;

			return sqlId;


		}


		Identifier MaxIDDataSet::getNextID(const CString& pObjectName)
		{

			MaxID* pMaxID = findMaxObject(pObjectName);
			if (NULL == pMaxID)
			{
				//there is no object with the speified name -> creating one
				pMaxID = new MaxID(pObjectName);
				setSQLID(pMaxID, ++m_iMaxID);
				m_ObjectList.insert(SQLObjectMap::value_type(pMaxID->getSQLID(), pMaxID));

			}

			pMaxID = findMaxObject(pObjectName);

			//this time object must exists
			ASSERT(pMaxID);
			return pMaxID->getNextID();


		}

		const Identifier& MaxIDDataSet::getCurrentMaxID(const CString& pObjectName)
		{

			MaxID* pMaxID = findMaxObject(pObjectName);

			//if user is asking for max id of "max id" object...
			CString sMaxIDRuntimeClassName(getRuntimeClass().m_lpszClassName);
			if (pObjectName == sMaxIDRuntimeClassName)
				return m_iMaxID;

			if (NULL != pMaxID)
				return pMaxID->getCurrentMaxID();

			return INVALID_SQL_ID;

		}


#if defined(_DEBUG) && defined(ALLOW_PERFORMANCE_ISSUES)
		void MaxIDDataSet::dumpMaxIDs()
		{

			SQLObjectMap::iterator it;
			for (it = m_ObjectList.begin(); it != m_ObjectList.end(); it++)
			{
				MaxID* pMaxID = (MaxID*)it->second;
				if (pMaxID)
					pMaxID->dumpMaxID();
			}

		}
#endif






	}; //namespace 
}