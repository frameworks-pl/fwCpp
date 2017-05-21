#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		bool SIDBToolkit::updateSingleObject(SISQLObject* pSQLObject, bool bSave, SIMaxIDDataSet* pMaxIDDS, SI_SQLT3_Connection* pConn)
		{

			if (pSQLObject)
			{

				//if we are storing and objectd is not modified - do nothing
				if ((true == bSave) && (false == pSQLObject->needsUpdate()))
					return false;

				db::SISQLObjectDataSet oTempDS(pSQLObject->GetRuntimeClass());
				oTempDS.initialize(pMaxIDDS);
				SQL_ID sqlID = INVALID_SQL_ID;
				if (true == bSave)
					sqlID = oTempDS.set(pSQLObject);

				if ((INVALID_SQL_ID != sqlID) || (false == bSave))
				{
					pConn->query(oTempDS, bSave);

					//if loading take first element from the collection
					if (oTempDS.initIterator())
					{
						const SISQLObject* pFirstSQLObject = oTempDS.getNextObject();
						pSQLObject->initialize(pFirstSQLObject);
						return true;
					}
				}
				else
				{
					CString s(oTempDS.getRuntimeClass().m_lpszClassName);
					CString sMsg;
					sMsg.Format(_T("Failed to load/store single object of class %s."), s);
					throw SIDBException(sMsg);
				}
			}

			return false;

		}


		CString SIDBToolkit::buildDeleteQueries(const CString& sTableName, const std::vector<SQL_ID>& oIDList)
		{
			CString sDeleteQueries;
			std::vector<SQL_ID>::const_iterator it;
			for (it = oIDList.begin(); it != oIDList.end(); it++)
			{
				CString sQuery;
				sQuery.Format(_T("DELETE FROM %s WHERE sql_id = %d;"), sTableName, (int)*it);
				sDeleteQueries += sQuery;
			}

			return sDeleteQueries;
		}







	}; //namespace 
}