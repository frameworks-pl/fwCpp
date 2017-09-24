#include "stdafx.h"
#include "fwarchive.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{
	namespace db
	{

		fw::crypt::Archive* SQLXMLDataItem::m_pArchive = NULL;
		CFile SQLXMLDataItem::m_FakeFile;

		SQLXMLDataItem::SQLXMLDataItem()
		{


		}

		bool SQLXMLDataItem::chunkToRootXML(const fw::core::ByteBuffer& pBuffer, XMLNode& pXmlDocument)
		{
			if (NULL == m_pArchive)
				throw fw::core::Exception(_T("SQL to XML engine not initialized."));

			CString xmlEntity = m_pArchive->decryptChunk(pBuffer);
#ifdef ALLOW_PERFORMANCE_ISSUES
			fw::debug::Logger::Log(LEVEL_SIDB_XML, "{SISQLXMLDataItem::chunkToRootXML} %s", fw::core::TextConv::Unicode2UTF8(xmlEntity).c_str());
#endif //ALLOW_PERFORMANCE_ISSUES
			pXmlDocument = XMLNode::parseString((LPCTSTR)xmlEntity);
			return true;


		}


		void SQLXMLDataItem::initializeArchive(const CString& pPassword)
		{
			if (NULL != m_pArchive)
				deinitializeArchive();


			m_pArchive = new fw::crypt::Archive(&m_FakeFile, 0);
			m_pArchive->setPassword((BYTE*)(LPCTSTR)pPassword, pPassword.GetLength());


		}

		void SQLXMLDataItem::deinitializeArchive()
		{
			delete m_pArchive;
			m_pArchive = NULL;
		}


		fw::core::ByteBuffer SQLXMLDataItem::encryptChunk(const CString& pChunk)
		{
			if (m_pArchive)
			{
				return m_pArchive->encryptChunk(pChunk);
			}

			throw fw::core::Exception(_T("SISQLXMLDataItem engine not initialized."));

		}



	}; //namespace 
}