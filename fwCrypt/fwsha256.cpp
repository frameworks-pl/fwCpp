#include "stdafx.h"
#include "fwsha256.h"
#include "fwcryptexception.h"
#include "src/fw/core/fwtextconv.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{

    namespace crypt {
    
    
    SHA256::SHA256() : mHash(CryptoPP::SHA256::DIGESTSIZE) {
    
      //mHash = NULL;
    
    }; //constructor
    
    /*
    void
    SISHA256::create(const CString& pPassPhrase)  {
    
      std::string pass_utf8 = Unicode2UTF8(pPassPhrase);
      CryptoPP::SHA256().CalculateDigest(mHash.getBuffer(), (BYTE*)pass_utf8.c_str(), pass_utf8.size());
    
    }; //constructor
    */
    
    
    void
    SHA256::create(BYTE* pBuffer, int pLength) {
    
      CryptoPP::SHA256().CalculateDigest(mHash.getBuffer(), pBuffer, pLength);
    
    }; //create
    
    
    void
    SHA256::init(BYTE* pHash, int pHashLen) {
    
      if (pHashLen != CryptoPP::SHA256::DIGESTSIZE) {
        CryptException ex(CryptoPP::Exception::OTHER_ERROR, "SISHA256 wrong length of the initialization buffer");
        throw ex;
      }
    
      memcpy(mHash.getBuffer(), pHash, CryptoPP::SHA256::DIGESTSIZE);
    
      
    
    }; //constructor
    
    
    bool 
    SHA256::verify(const fw::core::ByteBuffer& pPassPhrase) {
    
      if (mHash.getBuffer() == NULL) {
        CryptException ex(CryptoPP::Exception::OTHER_ERROR, "SISHA256::verify: verifying hash that haven't been initialized.");
        throw ex;
      }
    
      return (CryptoPP::SHA256().VerifyDigest(mHash.getBuffer(), pPassPhrase.getBuffer(), pPassPhrase.getLength()));
    
    }; //verify
    
    
    SHA256::~SHA256() {
    
    }; //destructor
    
    
    SHA256& 
    SHA256::operator=(const SHA256& pSHA256) {
    
      mHash = pSHA256.mHash;
      return *this;
    
    };
    
    
    SHA256::SHA256(const SHA256& pSHA256) {
    
      *this = pSHA256;
    
    }; //copy constructor
    
    
    
    int
    SHA256::getHashSize() { 
    
      return CryptoPP::SHA256::DIGESTSIZE; 
    
    }; //getHashSize
    
    
    BYTE* 
    SHA256::getBuffer() {
    
      if (mHash.getBuffer() == NULL) {
        CryptException ex(CryptoPP::Exception::OTHER_ERROR, "SISHA256::getBuffer: returning pointer to unitinialized hash.");
        throw ex;
      }
    
      return mHash.getBuffer();
    
    }; //getBuffer
    
    
    bool
    SHA256::operator==(const SHA256& pSHA256) {
    
      //if one of the hashes to compare is null
      //return false
      if ((mHash.getBuffer() && !pSHA256.mHash.getBuffer()) || (!mHash.getBuffer() && pSHA256.mHash.getBuffer()))
        return false;
    
      //if both hases are null - they are equal
      if (!mHash.getBuffer() && !pSHA256.mHash.getBuffer())
        return true;
    
      //comparing both hashes byte by byte to
      //detect any differences
      for (int i=0; i< CryptoPP::SHA256::DIGESTSIZE; i++) {
        if (mHash.getBuffer()[i] != pSHA256.mHash.getBuffer()[i])
          return false;
      }
    
      return true;
    
    }; //operator==


	CString SHA256::toBase64()
	{
		return b64encode(mHash.getBuffer(), mHash.getLength());
	}

	//creates sha256 from provided base64 string
	void SHA256::fromBase64(const CString& sBase64String)
	{
		mHash = b64decode(sBase64String);
	}

	static const char* B64chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	static const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
		56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
		7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
		0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

	CString SHA256::b64encode(const void* data, const size_t len)
	{
		unsigned char* p = (unsigned char*)data;
		size_t d = len % 3;
		std::string str64(4 * (int(d > 0) + len / 3), '=');

		for (size_t i = 0, j = 0; i < len - d; i += 3)
		{
			int n = int(p[i]) << 16 | int(p[i + 1]) << 8 | p[i + 2];
			str64[j++] = B64chars[n >> 18];
			str64[j++] = B64chars[n >> 12 & 0x3F];
			str64[j++] = B64chars[n >> 6 & 0x3F];
			str64[j++] = B64chars[n & 0x3F];
		}
		if (d--)    /// padding
		{
			int n = d ? int(p[len - 2]) << 8 | p[len - 1] : p[len - 1];
			str64[str64.size() - 2] = d ? B64chars[(n & 0xF) << 2] : '=';
			str64[str64.size() - 3] = d ? B64chars[n >> 4 & 0x03F] : B64chars[(n & 3) << 4];
			str64[str64.size() - 4] = d ? B64chars[n >> 10] : B64chars[n >> 2];
		}

		return fw::core::TextConv::UTF82Unicode(str64.c_str());
	}

	fw::core::ByteBuffer SHA256::b64decode(const CString& sBase64String)
	{
		std::string utf8Str = fw::core::TextConv::Unicode2UTF8(sBase64String);
		int len = (int)utf8Str.size();

		unsigned char* p = (unsigned char*)utf8Str.c_str();
		int pad = len > 0 && (len % 4 || p[len - 1] == '=');
		const size_t L = ((len + 3) / 4 - pad) * 4;
		std::string str(L / 4 * 3 + pad, '\0');

		for (size_t i = 0, j = 0; i < L; i += 4)
		{
			int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
			str[j++] = n >> 16;
			str[j++] = n >> 8 & 0xFF;
			str[j++] = n & 0xFF;
		}
		if (pad)
		{
			int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
			str[str.size() - 1] = n >> 16;

			if (len > L + 2 && p[L + 2] != '=')
			{
				n |= B64index[p[L + 2]] << 6;
				str.push_back(n >> 8 & 0xFF);
			}
		}

		fw::core::ByteBuffer buffer(str.size());
		memcpy(buffer.getBuffer(), str.c_str(), str.size());
		return buffer;
	}
    
    
    } //namespace crypt
    
} //namespace fw 