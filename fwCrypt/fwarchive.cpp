#include "stdafx.h"
#include "fwarchive.h"
#include "fwaesencdec.h"
#include <src/fw/core/fwdatetime.h>
#include <src/fw/core/fwtextconv.h>
#include <src/fw/core/fwnumbers.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw {

    namespace crypt {
    
    Archive::Archive( CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
    : CArchive(pFile, nMode, nBufSize, lpBuf) {
    
    
      mAES = NULL;
      setEmptyPassword();
    
    }; //constructor
    
    
    Archive::~Archive() {
    
      delete mAES;
    
    }; //destructor
    
    
    Archive& 
    Archive::operator>>(CString& pValue) {
    
      int enc_msg_len;
    
      //reading encrypted messagelength
      CArchive::operator>>(enc_msg_len);
    
      if (enc_msg_len > 0) {
      
        BYTE* pBuffer = new BYTE[enc_msg_len]; 
        Read(pBuffer, enc_msg_len);
    
        //decrypting
        //crypt::SIAESEncDec aes;
        //aes.init(mPassword, mPassLength);
        fw::core::ByteBuffer dec_message = mAES->decrypt(pBuffer, enc_msg_len);
    
        std::string dec_message_utf8((const char *)dec_message.getBuffer(), dec_message.getLength());
        pValue = fw::core::TextConv::UTF82Unicode(dec_message_utf8.c_str());
    
        delete[] pBuffer;
      }
    
      return *this;
    
    }; //operator>>(CString)
    
    
    Archive&
    Archive::operator<<(CString pValue) {
    
      //crypt::SIAESEncDec aes;
      //aes.init(mPassword, mPassLength);
    
      std::string enc_message_utf8 = fw::core::TextConv::Unicode2UTF8(pValue);
      fw::core::ByteBuffer enc_message = mAES->encrypt((BYTE*)enc_message_utf8.c_str(), (int)enc_message_utf8.size());
    
      //writing encrypted message len (this is open text, not sure if this is safe)
      CArchive::operator<<(enc_message.getLength());
      //writing the encrypted message
      Write(enc_message.getBuffer(), enc_message.getLength());
    
      return *this;
    
    }; //operator<<(CString)
    
    
    fw::core::ByteBuffer Archive::encryptChunk(const CString& pChunk)
    {
    
      std::string enc_message_utf8 = fw::core::TextConv::Unicode2UTF8(pChunk);
      fw::core::ByteBuffer enc_message = mAES->encrypt((BYTE*)enc_message_utf8.c_str(), (int)enc_message_utf8.size());
    
    
      return enc_message;
    
    }
    
    
    void Archive::encryptBuffer(const fw::core::ByteBuffer& pSrcBuffer, fw::core::ByteBuffer& pTrgtBuffer)
    {
    	pTrgtBuffer = mAES->encrypt(pSrcBuffer.getBuffer(), pSrcBuffer.getLength());
    }
    
    
    void Archive::decryptBuffer(const fw::core::ByteBuffer& pSrcBuffer, fw::core::ByteBuffer& pTrgtBuffer)
    {
        pTrgtBuffer = mAES->decrypt(pSrcBuffer.getBuffer(), pSrcBuffer.getLength());
    
    }
    
    
    CString Archive::decryptChunk(const fw::core::ByteBuffer& pChunk)
    {
    
      CString pValue;
      if (pChunk.getBuffer())
      {
    
        fw::core::ByteBuffer dec_message = mAES->decrypt(pChunk.getBuffer(), pChunk.getLength());
    
        std::string dec_message_utf8((const char *)dec_message.getBuffer(), dec_message.getLength());
        pValue = fw::core::TextConv::UTF82Unicode(dec_message_utf8.c_str());
      }
    
      return pValue;
    
    }
    
    
    
    
    Archive& 
    Archive::operator>>(int& pValue) {
    
      CString s;
      operator>>(s);
    
      pValue = fw::core::Numbers::toInt(s);
    
      return *this;
    
    }; //operator>>(int)
    
    
    Archive&
    Archive::operator<<(int pValue) {
    
      //this operator converts the integer
      //into string and then uses CString operator >>
      //to encrypt and save value!
    
      CString s;
      s.Format(_T("%d"), pValue);
      operator<<(s);
    
      return *this;
    
    }; //operator<<(int)
    
    
    void
    Archive::setPassword(BYTE* pBuffer, int pLength) {
    
      if (pLength > 0) {
        delete mAES;
        mAES = new crypt::AESEncDec;
        mAES->init(pBuffer, pLength);
      }
      else 
        setEmptyPassword();
    
    }; //setPassword
    
    
    void 
    Archive::setEmptyPassword() {
    
      delete mAES;
      mAES = new crypt::AESEncDec;
      mAES->init(NULL, 0);
    
    }; //setEmptyPassword

    } //namespace crypto
    
} //namespace fw