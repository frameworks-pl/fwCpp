#include "stdafx.h"
#include "GenericConnection.h"
#include "SQLObjectDataSet.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		GenericConnection::GenericConnection()
		{

		}

		void GenericConnection::destroyDeletedObjects(SQLObjectDataSet* pSQLObjectDataSet)
		{
			ASSERT(pSQLObjectDataSet);

			pSQLObjectDataSet->destroyDeletedObjects();

		}

	}; //namespace 
}