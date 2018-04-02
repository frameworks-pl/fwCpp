#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

#define BIND_SQLOBJECT_PARAMS addParam(new db::SQLParam(m_sSQLID_FieldName, &m_SQLID));

namespace fw
{
	namespace db
	{

		const CString SQLObject::m_sSQLID_FieldName = _T("sql_id");
		std::map<int, CString> SQLObject::m_StateToStringMap;

		SQLObject::SQLObject()
		{

			m_eState = STATE_NEW;
			m_SQLID = INVALID_SQL_ID;

			//default params that should always be there...
			BIND_SQLOBJECT_PARAMS
		}


		SQLObject::~SQLObject()
		{
			deleteSQLParams();
		}

		void SQLObject::deleteSQLParams()
		{
			ParamMap::iterator it;
			for (it = m_ParamMap.begin(); it != m_ParamMap.end(); it++)
			{
				delete it->second;
			}
			m_ParamMap.clear();
		}


		SQLObject::SQLObject(const SQLObject& pObject)
		{
			*this = pObject;
		}

		const SQLObject& SQLObject::operator=(const SQLObject& pObject)
		{
			m_eState = pObject.m_eState;
			m_SQLID = pObject.m_SQLID;

			deleteSQLParams();
			m_ObjectsToBind.clear();
			BIND_SQLOBJECT_PARAMS
			

			return *this;

		}


		void SQLObject::setDeleted()
		{
			m_eState = STATE_DELETE;

		}

		void SQLObject::setNew()
		{
			m_eState = STATE_NEW;
			m_SQLID = INVALID_SQL_ID;
		}

		bool SQLObject::isNew() const
		{
			return (STATE_NEW == m_eState);

		}


		bool SQLObject::initialize(const SQLObject* pObject)
		{
			m_eState = pObject->m_eState;
			m_SQLID = pObject->m_SQLID;

			return true;

		}

		void SQLObject::setSQLID(const SQL_ID& pSQLID)
		{
			if (INVALID_SQL_ID != m_SQLID)
			{
				throw DBException(_T("Assigning ID to an object that already has an ID."));
			}

			m_SQLID = pSQLID;
		}


		bool SQLObject::isDeleted() const
		{
			return (STATE_DELETE == m_eState);
		}

		bool SQLObject::isLoaded() const
		{
			return (STATE_OK_LOADED == m_eState);

		}

		bool SQLObject::needsUpdate() const
		{

			return ((m_eState != STATE_OK) && (m_eState != STATE_OK_LOADED));

		}


		void SQLObject::setUpdated()
		{

			//forcing update makes sense only on objects that exists already
			if (m_eState == STATE_OK || m_eState == STATE_OK_LOADED)
				m_eState = STATE_UPDATE;

		}


		void SQLObject::addParam(SQLParam* pParam)
		{
			if (pParam->getColumnName().IsEmpty())
				throw DBException(_T("Cannot add param to the collection - column name is empty."));


			//make sure there is no such column name already in the map
			ParamMap::iterator it = m_ParamMap.find(pParam->getColumnName());
			if (it != m_ParamMap.end())
				throw DBException(_T("Cannot add param to the colection - the param with the same column name is already there."));

			m_ParamMap.insert(ParamMap::value_type(pParam->getColumnName(), pParam));


		}


		bool SQLObject::removeParam(const CString& pName)
		{
			if (pName.IsEmpty())
				throw DBException(_T("Trying to remove a param with empty name."));

			ParamMap::iterator it = m_ParamMap.find(pName);
			if (it != m_ParamMap.end())
			{
				SQLParam* pParamToDelete = it->second;
				m_ParamMap.erase(it);
				delete pParamToDelete;
				return true;
			}

			return false;

		}


		int SQLObject::getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery)
		{
			pQuery = _T("");
			int iBindsCnt = 0; //number of bound variables
			ParamMap::const_iterator it;

			//reset the list of params to be bound
			m_ObjectsToBind.clear();


			if (bWrite)
			{
				switch (m_eState)
				{
				case STATE_NEW:
				{
					CString sColNames;
					CString sValues;
					CString sFormattedValue;

					for (it = m_ParamMap.begin(); it != m_ParamMap.end(); it++)
					{
						if (sColNames.IsEmpty())
							sColNames = it->second->getColumnName();
						else
							sColNames += _T(",") + it->second->getColumnName();

						bool bParamValue = it->second->getSQLFormattedValue(sFormattedValue);
						if (bParamValue)
						{
							iBindsCnt++;
							m_ObjectsToBind.insert(ParamVarsMap::value_type(iBindsCnt, it->second));
						}

						if (sValues.IsEmpty())
							sValues = sFormattedValue;
						else
							sValues += _T(",") + sFormattedValue;

					}

					pQuery.Format(_T("INSERT INTO %s(%s) VALUES(%s);"),
						getTableName(), sColNames, sValues);
				}
				break;
				case STATE_UPDATE:
				{
					CString sColVals;
					for (it = m_ParamMap.begin(); it != m_ParamMap.end(); it++)
					{
						CString sSinglePair;
						CString sFormattedValue;
						bool bParamValue = it->second->getSQLFormattedValue(sFormattedValue);
						if (bParamValue)
						{
							iBindsCnt++;
							m_ObjectsToBind.insert(ParamVarsMap::value_type(iBindsCnt, it->second));
						}

						sSinglePair.Format(_T("%s=%s"), it->second->getColumnName(), sFormattedValue);
						if (sColVals.IsEmpty())
							sColVals = sSinglePair;
						else
							sColVals += _T(",") + sSinglePair;
					}
					pQuery.Format(_T("UPDATE %s SET %s WHERE sql_id=%d"),
						getTableName(), sColVals, (int)getSQLID());
				}
				break;
				case STATE_DELETE:
				{
					pQuery.Format(_T("DELETE FROM %s WHERE sql_id=%d"),
						getTableName(), getSQLID());
				}
				break;
				case STATE_OK:
					throw db::DBException(_T("Asking for a query object that does not need storing."));
					break;
				}
			}
			else //reading
			{

				if (false == bCountQuery)
				{
					CString sColNames;
					for (it = m_ParamMap.begin(); it != m_ParamMap.end(); it++)
					{
						CString sFormattedValue;
						bool bParamValue = it->second->getSQLFormattedValue(sFormattedValue);
						if (bParamValue)
						{
							iBindsCnt++;
							m_ObjectsToBind.insert(ParamVarsMap::value_type(iBindsCnt, it->second));
						}

						if (sColNames.IsEmpty())
							sColNames = it->second->getColumnName();
						else
							sColNames += _T(",") + it->second->getColumnName();
					}
					pQuery.Format(_T("Select %s FROM %s"), sColNames, getTableName());
				}
				else
				{
					pQuery.Format(_T("Select count(*) FROM %s"), getTableName());
				}

			}

			return iBindsCnt;
		}


		const SQLParam* SQLObject::getParamToBind(int iIndex)
		{
			ParamVarsMap::iterator it = m_ObjectsToBind.find(iIndex);
			if (it != m_ObjectsToBind.end())
			{
				return it->second;
			}

			return &SQLParam::invalid();


		}


		void SQLObject::updateFromBLOB(const CString& pColName, const fw::core::ByteBuffer& pBLOB)
		{

			ParamMap::iterator it = m_ParamMap.find(pColName);
			if (it != m_ParamMap.end())
			{
				it->second->updateFromBLOB(pBLOB);
			}
			else
			{
				CString s;
				s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
				throw DBException(s);
			}

		}


		void SQLObject::updateFromInt(const CString& pColName, int iValue)
		{
			ParamMap::iterator it = m_ParamMap.find(pColName);
			if (it != m_ParamMap.end())
			{
				it->second->updateFromInt(iValue);
			}
			else
			{
				CString s;
				s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
				throw DBException(s);
			}

		}


		void SQLObject::updateFromString(const CString& pColName, const std::string& sValue)
		{

			ParamMap::iterator it = m_ParamMap.find(pColName);
			if (it != m_ParamMap.end())
			{
				it->second->updateFromString(sValue);
			}
			else
			{
				CString s;
				s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
				throw DBException(s);
			}

		}


		void SQLObject::init()
		{

			m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_NEW, _T("STATE_NEW")));
			m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_OK_LOADED, _T("STATE_OK_LOADED")));
			m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_OK, _T("STATE_OK")));
			m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_DELETE, _T("STATE_DELETE")));
			m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_UPDATE, _T("STATE_UPDATE")));

		}


		CString SQLObject::stateToString(int iState)
		{
			std::map<int, CString>::const_iterator it = m_StateToStringMap.find(iState);
			if (it != m_StateToStringMap.end())
			{
				return it->second;
			}

			ASSERT(false);
			return _T("Unknown state");


		}

	} //namespace 
}