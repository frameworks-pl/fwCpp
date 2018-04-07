#include "gtest.h"

#include <list>
#include <fwxmlnode.h>
#include <src/main/fwCommon.h>

#include <src/fw/core/fwexception.h>
#include <src/fw/core/fwgenericnotifier.h>
#include <src/fw/core/fwbytebuffer.h>
#include <src/fw/core/fwtextconv.h>
#include "dbpi.h"

#include <fwaesencdec.h>

//#include <src/fw/c

class TestObject : public fw::db::SQLObject
{
	public:
		TestObject(const CString& sName) : m_sName(sName) { }
		bool initialize() { return true; }
		CString getTableName() { return _T("test"); }
	protected:
		CString m_sName;

};


class TestDataSet : public fw::db::SQLObjectDataSet
{
	public:
		TestDataSet() : fw::db::SQLObjectDataSet(fw::db::SQLObjectDataSet(RUNTIME_CLASS(TestObject))) { }
};


//sicommon::SIByteBuffer SIArchive::encryptChunk(const CString& pChunk)
//{
//
//	std::string enc_message_utf8 = Unicode2UTF8(pChunk);
//	sicommon::SIByteBuffer enc_message = mAES->encrypt((BYTE*)enc_message_utf8.c_str(), (int)enc_message_utf8.size());
//
//
//	return enc_message;
//
//}

//if (pLength > 0) {
//	delete mAES;
//	mAES = new sicrypt::SIAESEncDec;
//	mAES->init(pBuffer, pLength);
//}

//    ar.setPassword((BYTE*)(LPCTSTR)mPassword, mPassword.GetLength());



TEST(SQLEncryptedParam, encryptDecryptTest) 
{

	
	CString sPassword = _T("password");
	std::string utf8Password = fw::core::TextConv::Unicode2UTF8(sPassword);
	fw::crypt::AESEncDec* aes = new fw::crypt::AESEncDec();
	aes->init((BYTE*)utf8Password.c_str(), utf8Password.size());

    CString sValue = _T("Value to encrypt");    

	fw::db::EncryptedSQLParam encryptedSQLParam(_T("column"), &sValue, aes);

	CString sEncryptedValue;
	encryptedSQLParam.getSQLFormattedValue(sEncryptedValue);
	sValue = _T("");

	CString s = sEncryptedValue.Mid(1, sEncryptedValue.GetLength()-2);
	std::string stdEncryptedString = fw::core::TextConv::Unicode2UTF8(s);

	encryptedSQLParam.updateFromString(stdEncryptedString);
	ASSERT_EQ(_T("Value to encrypt"), sValue);

}


TEST(SQLEncryptedParam, encryptEmptyValue)
{
	CString sPassword = _T("password");
	std::string utf8Password = fw::core::TextConv::Unicode2UTF8(sPassword);
	fw::crypt::AESEncDec* aes = new fw::crypt::AESEncDec();
	aes->init((BYTE*)utf8Password.c_str(), utf8Password.size());

	CString sValue = _T("");

	fw::db::EncryptedSQLParam encryptedSQLParam(_T("column"), &sValue, aes);

	CString sEncryptedValue;
	encryptedSQLParam.getSQLFormattedValue(sEncryptedValue);
	sValue = _T("");

	CString s = sEncryptedValue.Mid(1, sEncryptedValue.GetLength() - 2);
	std::string stdEncryptedString = fw::core::TextConv::Unicode2UTF8(s);

	encryptedSQLParam.updateFromString(stdEncryptedString);
	ASSERT_EQ(_T(""), sValue);
}