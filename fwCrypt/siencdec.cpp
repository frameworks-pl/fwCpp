#include "stdafx.h"
#include "siencdec.h"

#pragma warning (push)
#pragma warning (disable: 4267)
#include <crypto5/hex.h>
#pragma warning (pop)

#include "sicryptexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{

    namespace crypt {
    
    
    EncDec::EncDec() {
    
      mPrivateKey = NULL;
      mPublicKey = NULL;
    
      //mEncryptedMessageLength = 0;
      //mDecryptedMessageLength = 0;
    
    }; //constructor
    
    EncDec::~EncDec() {
    
      delete mPrivateKey;
      delete mPublicKey;
    
    }; //destructor
    
    
    fw::core::ByteBuffer
    EncDec::hexEncode(BYTE* pData, UINT pLen) {
    
      fw::core::ByteBuffer output(pLen*2);
    
      try {
        CryptoPP::HexEncoder encoder;
        encoder.Put((BYTE *)pData, pLen);
        encoder.MessageEnd();
        encoder.Get(output.getBuffer(), output.getLength());
      }
      catch (CryptoPP::Exception& ex) {
        TRACE(_T("SIEncDec::hexEncode: %s"), ex.what());
        throw CryptException(ex.GetErrorType(), ex.GetWhat());
      }
      return output;
    
    }; //hexEncode
    
    fw::core::ByteBuffer
    EncDec::hexDecode(BYTE* pData, UINT pLen) {
    
      fw::core::ByteBuffer output;
    
      try {
        //decrypt message from hex
        CryptoPP::HexDecoder decoder;
    	  decoder.Put((byte *)pData, pLen);
    	  decoder.MessageEnd();
        ASSERT(pLen % 2 == 0); //length mast be even
        output.resize(pLen/2);
    	  decoder.Get(output.getBuffer(), output.getLength()); //hex uses two bytes to encode one char!!!
      }
      catch(CryptoPP::Exception& ex) {
        TRACE(_T("SIEncDec::hexDecode: %s"), ex.what());
        throw CryptException(ex.GetErrorType(), ex.GetWhat());
      }
      return output;
    
    }; //hexDecode
    
    
    void 
    EncDec::xor(BYTE* pData, UINT pLen, BYTE pMask) {
    
      BYTE* pStart = pData;
    
      while (pLen) {
        *pStart = *pStart ^ pMask;
        pLen--;
        pStart++;
      }
    
    }; //xor
    
    
    void
    EncDec::xorMoving(BYTE* pData, UINT pLen)
    {
    	//the buffer must have at least two bytes
    	if (pLen > 0)
    	{
    		BYTE mask = 0;
    		UINT count = pLen;
    		BYTE* pCurrent = pData;
    		while (count)
    		{
    			*pCurrent = *pCurrent ^ mask;
    			mask++;
    
    			count--;
    			pCurrent++;
    		}
    
    	}
    
    }
    
    } //namespace crypt

} //namespace fw