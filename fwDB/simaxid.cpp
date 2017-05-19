#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sidb
{

CString SIMaxID::m_sTableName = _T("max_ids"); //default value of this table should be changed

IMPLEMENT_DYNCREATE(SIMaxID, SISQLXMLDataItem)

SIMaxID::SIMaxID() 
{

  m_iUniqueID = INVALID_SQL_ID;
  //we want the id to be valid from the very beginning
  m_iUniqueID++;

  addParam(SISQLParam(_T("data"), &m_sql_maxid_data));

}

SIMaxID::SIMaxID(const CString& pMaxIDName)
{

  m_sName = pMaxIDName;

  m_iUniqueID = INVALID_SQL_ID;
  //we want the id to be valid from the very beginning
  m_iUniqueID++;

  addParam(SISQLParam(_T("data"), &m_sql_maxid_data));


}


Identifier SIMaxID::getNextID()
{
  Identifier new_id = m_iUniqueID;
  m_iUniqueID++;

  //we have to enforce update here
  setUpdated();

  return new_id;


}


Identifier SIMaxID::getCurrentMaxID() const
{
  return m_iUniqueID;

}


bool SIMaxID::initialize()
{

  if (isLoaded() && m_sql_maxid_data.getLength() > 0)
  {
    XMLNode xmlDoc;
    if (chunkToRootXML(m_sql_maxid_data, xmlDoc))
        return fromUnicodeXML(&xmlDoc);
  }

  return true;

}


bool SIMaxID::initialize(const SISQLObject* pObject)
{
  SISQLXMLDataItem::initialize(pObject);
  ASSERT(sicommon::SIAppUtils::runtimeObjectsHaveTheSameClass((CObject*)pObject, this));

  *this = *(const SIMaxID*)pObject;
  return initialize();


}


XMLNode SIMaxID::toUnicodeXML() const
{

  XMLNode el = XMLNode::createXMLTopNode(_T("sidb_maxid"));
  SIXMLNode::addAttribute(el, _T("name"), m_sName);
  SIXMLNode::addAttribute(el, _T("unique_id"), m_iUniqueID);

#ifdef ALLOW_PERFORMANCE_ISSUES
  SILog::Log(LEVEL_SIDB_MAXID, "{SIMaxID::toUnicodeXML} m_sName=%s, m_iUniqueID=%d",
    Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);
#endif

  return el;

}


bool SIMaxID::fromUnicodeXML(const XMLNode* pXmlElement)
{

  const wchar_t* pAttr = pXmlElement->getAttribute(_T("name"));
  if (!pAttr)
    return false;
  m_sName = CString(pAttr);


  int iID;
  if (false == SIXMLNode::getAttributeInt(*pXmlElement, _T("unique_id"), iID))
    return false;
  m_iUniqueID = (Identifier)iID;

#ifdef ALLOW_PERFORMANCE_ISSUES
  SILog::Log(LEVEL_SIDB_MAXID, "{SIMaxID::fromUnicodeXML} m_sName=%s, m_iUniqueID=%d", 
    Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);
#endif

  return true;

}

int SIMaxID::getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery)
{
  if (true == bWrite)
  {
    
    if (STATE_NEW == getState() || STATE_UPDATE == getState())
    {
#ifdef USE_UNICODE_XML

      XMLNode xmlEntity = toUnicodeXML();
      CString xmlData = SIXMLNode::nodeToString(xmlEntity);
#else
      TiXmlElement xmlEntity = toXML();
      std::string utf8Xml;
      utf8Xml << xmlEntity;
      CString xmlData = UTF82Unicode(utf8Xml.c_str());
#endif
      m_sql_maxid_data = encryptChunk(xmlData);
    }
  }

  return sidb::SISQLXMLDataItem::getUpdateQuery(pQuery, bWrite, bCountQuery);

}


#if defined(_DEBUG) && defined(ALLOW_PERFORMANCE_ISSUES)
void SIMaxID::dumpMaxID()
{
  SILog::Log(LEVEL_SIDB, "m_sName=%s, m_iUniqueID=%d", Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);

}
#endif


void SIMaxID::initMaxIDS(const CString& pTableName)
{
  m_sTableName = pTableName;

}

}; //namespace