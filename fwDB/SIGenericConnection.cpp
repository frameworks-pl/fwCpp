#include "stdafx.h"
#include "SIGenericConnection.h"
#include "SISQLObjectDataSet.h"

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