#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		SQLRowDataSet::SQLRowDataSet(const CString& sReadQuery)
		{
			m_sReadQuery = sReadQuery;

		}


		void SQLRowDataSet::append(const SQLRow& pRow)
		{
			m_RowList.push_back(pRow);

		}


	} //namespace 
}