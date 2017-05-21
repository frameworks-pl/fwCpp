#include "stdafx.h"
#include "siaesencdec.h"
#include "aesphmencryption.h"
#include "aesphmdecryption.h"
#include "sicryptexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace fw 
{

    namespace crypt {
    
    fw::core::ByteBuffer
    AESEncDec::encrypt(const BYTE* pData, int pLen) {
    
      fw::core::ByteBuffer output;
      try {
        CryptoPP::AutoSeededRandomPool randPool;
        AES_PHM_Encryption encryptor(randPool, mPrivateKey, mPrivateKeyLen);
    
        encryptor.Put(pData, pLen);
        encryptor.MessageEnd();
    
        output.resize(encryptor.MaxRetrievable());
        encryptor.Get(output.getBuffer(), output.getLength());
      }
      catch (CryptoPP::Exception& ex) {
        crypt::CryptException e(ex.GetErrorType(), ex.GetWhat());
        throw e;
      }
    
      return output;
    
    }; //encrypt
    
    
    fw::core::ByteBuffer
    AESEncDec::decrypt(const BYTE* pData, int pLen) {
    
      fw::core::ByteBuffer output;
      try {
    
        AES_PHM_Decryption decryptor(mPrivateKey, mPrivateKeyLen);
        decryptor.Put(pData, pLen);
        decryptor.MessageEnd();
        output.resize(decryptor.MaxRetrievable());
        decryptor.Get(output.getBuffer(), output.getLength());
      }
      catch (CryptoPP::Exception& ex) {
        crypt::CryptException e(ex.GetErrorType(), ex.GetWhat());
        throw e;
      }
    
      return output;
    
    }; //decrypt
    
    /*
    void 
    SIAESEncDec::init(const CString& pPassword) {
    
        //destroy the current key (if one already exists)
        delete mPrivateKey;
        mPrivateKey = NULL;
        
        mPrivateKeyLen = pPassword.GetLength();
    
        if (mPrivateKeyLen > 0) {
          mPrivateKey = new BYTE[mPrivateKeyLen];
          memcpy(mPrivateKey, (LPCTSTR)pPassword, mPrivateKeyLen);
        }
    
    
    
    }; //init
    */
    
    
    void
    AESEncDec::init(BYTE* pBuffer, int pLength) {
    
        //destroy the current key (if one already exists)
        delete mPrivateKey;
        mPrivateKey = NULL;
        
        mPrivateKeyLen = pLength;
    
        if (mPrivateKeyLen > 0) {
          mPrivateKey = new BYTE[mPrivateKeyLen];
          memcpy(mPrivateKey, pBuffer, mPrivateKeyLen);
        }
    
    }; //init
    
    
    } //namespace crypto
    
} //namespace fw