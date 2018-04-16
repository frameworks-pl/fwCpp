#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		CString MaxID::m_sTableName = _T("max_ids"); //default value of this table should be changed

		IMPLEMENT_DYNCREATE(MaxID, SQLXMLDataItem)

			MaxID::MaxID()
		{

			m_iUniqueID = INVALID_SQL_ID;
			//we want the id to be valid from the very beginning
			m_iUniqueID++;

			addParam(new SQLParam(_T("data"), &m_sql_maxid_data));

		}

		MaxID::MaxID(const CString& pMaxIDName)
		{

			m_sName = pMaxIDName;

			m_iUniqueID = INVALID_SQL_ID;
			//we want the id to be valid from the very beginning
			m_iUniqueID++;

			addParam(new SQLParam(_T("data"), &m_sql_maxid_data));


		}

		MaxID::MaxID(const MaxID& pObject)
		{
			*this = pObject;
		}

		const MaxID& MaxID::operator=(const MaxID& pObject)
		{
			parent::operator=(pObject);

			m_iUniqueID = pObject.m_iUniqueID;
			m_sName = pObject.m_sName;
			m_sql_maxid_data = pObject.m_sql_maxid_data;

			return *this;
		}


		Identifier MaxID::getNextID()
		{
			fw::db::Identifier new_id = m_iUniqueID;
			m_iUniqueID++;

			//we have to enforce update here
			setUpdated();

			return new_id;


		}


		Identifier MaxID::getCurrentMaxID() const
		{
			return m_iUniqueID;

		}


		bool MaxID::initialize()
		{

			if (isLoaded() && m_sql_maxid_data.getLength() > 0)
			{
				XMLNode xmlDoc;
				if (chunkToRootXML(m_sql_maxid_data, xmlDoc))
					return fromUnicodeXML(&xmlDoc);
			}

			return true;

		}


		bool MaxID::initialize(const SQLObject* pObject)
		{
			SQLXMLDataItem::initialize(pObject);
			ASSERT(fw::core::AppUtils::runtimeObjectsHaveTheSameClass((CObject*)pObject, this));

			*this = *(const MaxID*)pObject;
			return initialize();


		}


		XMLNode MaxID::toUnicodeXML() const
		{

			XMLNode el = XMLNode::createXMLTopNode(_T("sidb_maxid"));
			FWXMLNode::addAttribute(el, _T("name"), m_sName);
			FWXMLNode::addAttribute(el, _T("unique_id"), m_iUniqueID);

#ifdef ALLOW_PERFORMANCE_ISSUES
			fw::debug::Logger::Log(LEVEL_SIDB_MAXID, "{SIMaxID::toUnicodeXML} m_sName=%s, m_iUniqueID=%d",
				fw::core::TextConv::Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);
#endif

			return el;

		}


		bool MaxID::fromUnicodeXML(const XMLNode* pXmlElement)
		{

			const wchar_t* pAttr = pXmlElement->getAttribute(_T("name"));
			if (!pAttr)
				return false;
			m_sName = CString(pAttr);


			int iID;
			if (false == FWXMLNode::getAttributeInt(*pXmlElement, _T("unique_id"), iID))
				return false;
			m_iUniqueID = (Identifier)iID;

#ifdef ALLOW_PERFORMANCE_ISSUES
			fw::debug::Logger::Log(LEVEL_SIDB_MAXID, "{SIMaxID::fromUnicodeXML} m_sName=%s, m_iUniqueID=%d",
				fw::core::TextConv::Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);
#endif

			return true;

		}

		int MaxID::getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery)
		{
			if (true == bWrite)
			{

				if (STATE_NEW == getState() || STATE_UPDATE == getState())
				{
#ifdef USE_UNICODE_XML

					XMLNode xmlEntity = toUnicodeXML();
					CString xmlData = FWXMLNode::nodeToString(xmlEntity);
#else
					TiXmlElement xmlEntity = toXML();
					std::string utf8Xml;
					utf8Xml << xmlEntity;
					CString xmlData = fw::core::TextConv::UTF82Unicode(utf8Xml.c_str());
#endif
					m_sql_maxid_data = encryptChunk(xmlData);
				}
			}

			return db::SQLXMLDataItem::getUpdateQuery(pQuery, bWrite, bCountQuery);

		}


#if defined(_DEBUG) && defined(ALLOW_PERFORMANCE_ISSUES)
		void MaxID::dumpMaxID()
		{
			fw::debug::Logger::Log(LEVEL_SIDB, "m_sName=%s, m_iUniqueID=%d", fw::core::TextConv::Unicode2UTF8(m_sName).c_str(), (int)m_iUniqueID);

		}
#endif


		void MaxID::initMaxIDS(const CString& pTableName)
		{
			m_sTableName = pTableName;

		}

	}; //namespace 
}