#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sidb
{

const CString SISQLObject::m_sSQLID_FieldName = _T("sql_id");
std::map<int, CString> SISQLObject::m_StateToStringMap;

SISQLObject::SISQLObject()
{

  m_eState = STATE_NEW;
  m_SQLID = INVALID_SQL_ID;

  //default params that should always be there...
  addParam(sidb::SISQLParam(m_sSQLID_FieldName, &m_SQLID));

}


SISQLObject::SISQLObject(const SISQLObject& pObject)
{
    *this = pObject;
}

const SISQLObject& SISQLObject::operator=(const SISQLObject& pObject)
{
  m_eState = pObject.m_eState;
  m_SQLID = pObject.m_SQLID;

  return *this;

}


void SISQLObject::setDeleted()
{
  m_eState = STATE_DELETE;

}

void SISQLObject::setNew()
{
  m_eState = STATE_NEW;
  m_SQLID = INVALID_SQL_ID;
}

bool SISQLObject::isNew() const
{
    return (STATE_NEW == m_eState);

}


bool SISQLObject::initialize(const SISQLObject* pObject)
{
  m_eState = pObject->m_eState;
  m_SQLID = pObject->m_SQLID;

  return true;

}

void SISQLObject::setSQLID(const SQL_ID& pSQLID)
{
  if (INVALID_SQL_ID != m_SQLID )
  {
    throw SIDBException(_T("Assigning ID to an object that already has an ID."));
  }

  m_SQLID = pSQLID;
}


bool SISQLObject::isDeleted() const 
{ 
  return (STATE_DELETE == m_eState); 
}

bool SISQLObject::isLoaded() const
{
  return (STATE_OK_LOADED == m_eState);

}

bool SISQLObject::needsUpdate() const
{

  return ((m_eState != STATE_OK) && (m_eState != STATE_OK_LOADED));

}


void SISQLObject::setUpdated()
{

  //forcing update makes sense only on objects that exists already
  if (m_eState == STATE_OK || m_eState == STATE_OK_LOADED)
    m_eState = STATE_UPDATE;

}


void SISQLObject::addParam(const SISQLParam& pParam)
{
  if (pParam.getColumnName().IsEmpty())
    throw SIDBException(_T("Cannot add param to the collection - column name is empty."));


  //make sure there is no such column name already in the map
  ParamMap::iterator it = m_ParamMap.find(pParam.getColumnName());
  if (it != m_ParamMap.end())
    throw SIDBException(_T("Cannot add param to the colection - the param with the same column name is already there."));

  m_ParamMap.insert(ParamMap::value_type(pParam.getColumnName(), pParam));


}


bool SISQLObject::removeParam(const CString& pName)
{
	if (pName.IsEmpty())
		throw SIDBException(_T("Trying to remove a param with empty name."));

	ParamMap::iterator it = m_ParamMap.find(pName);
	if (it != m_ParamMap.end())
	{
		m_ParamMap.erase(it);
		return true;
	}

	return false;

}


int SISQLObject::getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery)
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
              sColNames = it->second.getColumnName();
            else
              sColNames += _T(",") + it->second.getColumnName();

            bool bParamValue = it->second.getSQLFormattedValue(sFormattedValue);
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
            bool bParamValue = it->second.getSQLFormattedValue(sFormattedValue);
            if (bParamValue)
            {
              iBindsCnt++;
              m_ObjectsToBind.insert(ParamVarsMap::value_type(iBindsCnt, it->second));
            }

            sSinglePair.Format(_T("%s=%s"), it->second.getColumnName(), sFormattedValue);
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
        throw sidb::SIDBException(_T("Asking for a query object that does not need storing."));
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
        bool bParamValue = it->second.getSQLFormattedValue(sFormattedValue);
        if (bParamValue)
        {
          iBindsCnt++;
          m_ObjectsToBind.insert(ParamVarsMap::value_type(iBindsCnt, it->second));
        }

        if (sColNames.IsEmpty())
          sColNames = it->second.getColumnName();
        else
          sColNames += _T(",") + it->second.getColumnName();
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


const SISQLParam& SISQLObject::getParamToBind(int iIndex)
{
  ParamVarsMap::iterator it = m_ObjectsToBind.find(iIndex);
  if (it != m_ObjectsToBind.end())
  {
    return it->second;
  }

  return SISQLParam::invalid();


}


void SISQLObject::updateFromBLOB(const CString& pColName, const fw::core::ByteBuffer& pBLOB)
{

  ParamMap::iterator it = m_ParamMap.find(pColName);
  if (it != m_ParamMap.end())
  {
    it->second.updateFromBLOB(pBLOB);
  }
  else
  {
    CString s;
    s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
    throw SIDBException(s);
  }

}


void SISQLObject::updateFromInt(const CString& pColName, int iValue)
{
  ParamMap::iterator it = m_ParamMap.find(pColName);
  if (it != m_ParamMap.end())
  {
    it->second.updateFromInt(iValue);
  }
  else
  {
    CString s;
    s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
    throw SIDBException(s);
  }

}


void SISQLObject::updateFromString(const CString& pColName, const CString sValue)
{

  ParamMap::iterator it = m_ParamMap.find(pColName);
  if (it != m_ParamMap.end())
  {
    it->second.updateFromString(sValue);
  }
  else
  {
    CString s;
    s.Format(_T("Fatal error. Param %s not found in the object."), pColName);
    throw SIDBException(s);
  }

}


void SISQLObject::init()
{

  m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_NEW, _T("STATE_NEW")));
  m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_OK_LOADED, _T("STATE_OK_LOADED")));
  m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_OK, _T("STATE_OK")));
  m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_DELETE, _T("STATE_DELETE")));
  m_StateToStringMap.insert(std::map<int, CString>::value_type(STATE_UPDATE, _T("STATE_UPDATE")));

}


CString SISQLObject::stateToString(int iState)
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