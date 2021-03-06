#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		SQLRow::SQLRow()
		{

		}


		void SQLRow::addValue(const char* pColName, const char* pColValue)
		{
			if (NULL == pColName)
				throw DBException(_T("Unknown column name when parsing a row."));

			CString sColName = fw::core::TextConv::UTF82Unicode(pColName);
			CString sColValue = fw::core::TextConv::UTF82Unicode(pColValue);

			m_ValuesMap.insert(NameValueMap::value_type(sColName, sColValue));

		}


		int SQLRow::getValueAsInt(const CString& pColName) const
		{

			int iResult = 0;

			NameValueMap::const_iterator it = m_ValuesMap.find(pColName);
			if (it != m_ValuesMap.end())
			{
				if (false == fw::core::Numbers::isInt(it->second, true))
				{
					CString sMsg;
					sMsg.Format(_T("Cannot convert %s to integer."), it->second);
					throw db::DBException(sMsg);
				}
			}

			iResult = fw::core::Numbers::toInt(it->second);
			return iResult;

		}


		int SQLRow::getValueAsInt(int iColIndex) const
		{
			int iResult = 0;

			//find column
			int iIndex = 0;
			NameValueMap::const_iterator it;
			for (it = m_ValuesMap.begin(); it != m_ValuesMap.end(); it++)
			{
				if (iIndex == iColIndex)
					return getValueAsInt(it->first);
			}

			CString sMsg;
			sMsg.Format(_T("Index %d is out of range."), iColIndex);
			throw DBException(sMsg);

		}



	}; //namespace 
}