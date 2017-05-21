#include "stdafx.h"
#include "sisha256.h"
#include "sicryptexception.h"

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
    
    
    } //namespace crypt
    
} //namespace fw 