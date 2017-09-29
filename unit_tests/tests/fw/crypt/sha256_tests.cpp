#include "gtest.h"
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwtextconv.h>
#include <src/fw/core/fwbytebuffer.h>
#include <fwsha256.h>



TEST(SHA256, base64EncodeDecode)
{
	CString sOpenText(_T("password"));
	std::string sOpenTextUTF8 = fw::core::TextConv::Unicode2UTF8(sOpenText);
	fw::core::ByteBuffer buffer((int)sOpenTextUTF8.size());
	memcpy(buffer.getBuffer(), sOpenTextUTF8.c_str(), sOpenTextUTF8.size());

	CString encoded = fw::crypt::SHA256::b64encode(buffer.getBuffer(), buffer.getLength());
	fw::core::ByteBuffer bufferDecoded = fw::crypt::SHA256::b64decode(encoded);
	std::string utf8Decoded((char*)bufferDecoded.getBuffer(), bufferDecoded.getLength());
	CString sDecded = fw::core::TextConv::UTF82Unicode(utf8Decoded.c_str());

	ASSERT_EQ(_T("password"), sDecded);

}



TEST(SHA256, sha256ToBase64)
{
	CString sOpenText(_T("password"));
	std::string sOpenTextUTF8 = fw::core::TextConv::Unicode2UTF8(sOpenText);
	fw::core::ByteBuffer buffer((int)sOpenTextUTF8.size());
	memcpy(buffer.getBuffer(), sOpenTextUTF8.c_str(), sOpenTextUTF8.size());
	fw::crypt::SHA256 sha256;
	sha256.create(buffer.getBuffer(), buffer.getLength());

	CString hashBase64 = sha256.toBase64();

	fw::crypt::SHA256 sha256_2;
	sha256_2.fromBase64(hashBase64);
	
	ASSERT_TRUE(sha256_2.verify(buffer));
}
