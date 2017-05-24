#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace fw
{
	namespace db
	{

		SQLParam::SQLParam()
		{

			init();

		}

		SQLParam::SQLParam(const CString& pColName, int* pIntValue)
		{
			init();
			m_sColumnName = pColName;
			m_eParamType = PARAM_INTEGER;
			m_pIntValue = pIntValue;


		}

		SQLParam::SQLParam(const CString& pColName, CString* pStringValue)
		{
			init();
			m_sColumnName = pColName;
			m_eParamType = PARAM_STRING;
			m_pStringValue = pStringValue;

		}

		SQLParam::SQLParam(const CString& pColName, fw::core::ByteBuffer* pByteBuffer)
		{
			init();
			m_sColumnName = pColName;
			m_eParamType = PARAM_BLOB;
			m_pByteBuffer = pByteBuffer;

		}

		SQLParam::SQLParam(const CString& pColName, db::BLOBItem* pBLOBItem)
		{
			init();
			m_sColumnName = pColName;
			m_eParamType = PARAM_BLOB_POINTER;
			m_pBLOBItem = pBLOBItem;

		}


		void SQLParam::init()
		{

			m_eParamType = PARAM_UNKNOWN;
			m_pIntValue = NULL;
			m_pStringValue = NULL;
			m_pByteBuffer = NULL;
			m_pBLOBItem = NULL;


		}


		bool SQLParam::getSQLFormattedValue(CString& pFormattedValue) const
		{

			bool bHasParam = false;
			switch (m_eParamType)
			{
			case PARAM_INTEGER:
				pFormattedValue.Format(_T("%d"), *m_pIntValue);
				break;
			case PARAM_STRING:
				pFormattedValue.Format(_T("'%s'"), *m_pStringValue);
				break;
			case PARAM_BLOB:
			case PARAM_BLOB_POINTER:
				pFormattedValue.Format(_T("?"));
				bHasParam = true;
				break;

			default:
				throw db::DBException(_T("Trying to format unknown type of param."));

			}

			return bHasParam;

		}


		const SQLParam& SQLParam::invalid()
		{
			static SQLParam invalidParam;
			return invalidParam;

		}
		bool SQLParam::isValid() const
		{
			if (PARAM_UNKNOWN == m_eParamType)
				return false;

			switch (m_eParamType)
			{
			case PARAM_BLOB_POINTER:
				return (NULL != m_pBLOBItem);
			case PARAM_BLOB:
				return (NULL != m_pByteBuffer);
			case PARAM_INTEGER:
				return (NULL != m_pIntValue);
			case PARAM_STRING:
				return (NULL != m_pStringValue);
			default:
				ASSERT(false);
				return false;
			}

			return false;

		}


		const fw::core::ByteBuffer& SQLParam::getBLOBBuffer() const
		{

			switch (m_eParamType)
			{
			case PARAM_BLOB:
				if (NULL == m_pByteBuffer)
					throw DBException(_T("BLOB param is invalid."));
				return *m_pByteBuffer;
				break;
			case PARAM_BLOB_POINTER:
				if (NULL == m_pBLOBItem || NULL == m_pBLOBItem->getDataBuffer())
					throw DBException(_T("BLOB pointer param is invalid."));
				return *m_pBLOBItem->getDataBuffer();
				break;
			default:
				throw DBException(_T("Param is invalid."));
			}


			ASSERT(FALSE);
			return NULL;
		}


		void SQLParam::updateFromBLOB(const fw::core::ByteBuffer& pBLOB)
		{

			if (PARAM_BLOB != m_eParamType && PARAM_BLOB_POINTER != m_eParamType)
				throw DBException(_T("Trying to initialize non-BLOB param from a BLOB."));

			if (PARAM_BLOB == m_eParamType)
				*m_pByteBuffer = pBLOB;
			else if (PARAM_BLOB_POINTER == m_eParamType)
			{
				fw::core::ByteBuffer* pByteBuffer = new fw::core::ByteBuffer(pBLOB);
				*m_pBLOBItem = BLOBItem(pByteBuffer);
			}

		}


		void SQLParam::updateFromInt(int pIntValue)
		{

			if (PARAM_INTEGER != m_eParamType)
				throw DBException(_T("Trying to initialize non-INT param from an integer value."));

			*m_pIntValue = pIntValue;

		}


		void SQLParam::updateFromString(const CString& pStringValue)
		{
			if (PARAM_STRING != m_eParamType)
				throw DBException(_T("Trying to initialize non-STRING param from a string value."));

			*m_pStringValue = pStringValue;

		}





	} //namespace 
}