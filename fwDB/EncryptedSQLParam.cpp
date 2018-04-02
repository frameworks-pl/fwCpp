#include "stdafx.h"
#include "EncryptedSQLParam.h"
#include <fwsha256.h>

namespace fw
{
	namespace db
	{
		//constructs integer sql param
		EncryptedSQLParam::EncryptedSQLParam(const CString& pColName, int* pIntValue, fw::crypt::EncDec* pCipher)
			: SQLParam(pColName, pIntValue), m_pCipher(pCipher)
		{
		}

		//constructs string sql param
		EncryptedSQLParam::EncryptedSQLParam(const CString& pColName, CString* pStringValue, fw::crypt::EncDec* pCipher)
			: SQLParam(pColName, pStringValue), m_pCipher(pCipher)
		{

		}

		//constructs blob sql param
		EncryptedSQLParam::EncryptedSQLParam(const CString& pColName, fw::core::ByteBuffer* pByteBuffer, fw::crypt::EncDec* pCipher)
			: SQLParam(pColName, pByteBuffer), m_pCipher(pCipher)
		{
		}

		//constructs blob pointer sql param
		EncryptedSQLParam::EncryptedSQLParam(const CString& pColName, BLOBItem* pBLOBItem, fw::crypt::EncDec* pCipher)
			: SQLParam(pColName, pBLOBItem), m_pCipher(pCipher)
		{
		}

		bool EncryptedSQLParam::getSQLFormattedValue(CString& pFormattedValue) const
		{
			bool bHasParam = false;
			switch (m_eParamType)
			{
				case PARAM_STRING:
				{
					//	std::string enc_message_utf8 = Unicode2UTF8(pChunk);
					//	sicommon::SIByteBuffer enc_message = mAES->encrypt((BYTE*)enc_message_utf8.c_str(), (int)enc_message_utf8.size());

					std::string message_utf8 = fw::core::TextConv::Unicode2UTF8(*m_pStringValue);
					fw::core::ByteBuffer enc_message = m_pCipher->encrypt((BYTE*)message_utf8.c_str(), (int)message_utf8.size());
					CString b64_encoded = fw::crypt::SHA256::b64encode(enc_message.getBuffer(), enc_message.getLength());
					pFormattedValue.Format(_T("'%s'"), b64_encoded);
				}
					break;
				default:
					throw db::DBException(_T("Attempting to use encrypted param for unsupported value type."));
			}
		}

		void EncryptedSQLParam::updateFromString(const std::string& pStringValue)
		{
			if (PARAM_STRING != m_eParamType)
				throw DBException(_T("Trying to initialize non-STRING param from a string value."));

			CString b64_encoded = fw::core::TextConv::UTF82Unicode(pStringValue.c_str());
			fw::core::ByteBuffer enc_message =  fw::crypt::SHA256::b64decode(b64_encoded);
			fw::core::ByteBuffer dec_message = m_pCipher->decrypt(enc_message.getBuffer(), enc_message.getLength());			
			std::string dec_message_utf8((const char *)dec_message.getBuffer(), dec_message.getLength());
			*m_pStringValue = fw::core::TextConv::UTF82Unicode(dec_message_utf8.c_str());

		}
	}


}