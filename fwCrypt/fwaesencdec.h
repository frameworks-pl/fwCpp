#ifndef SIAESENCDEC_H_INCLUDED
#define SIAESENCDEC_H_INCLUDED

#include "fwencdec.h"


namespace fw
{

    namespace crypt {
    
    /**
      @brief AES PHM Encyrption/Decryption class 
    */
    
    class AESEncDec : public EncDec {
    
      public:
        fw::core::ByteBuffer encrypt(const BYTE* pData, int pLen);
        fw::core::ByteBuffer decrypt(const BYTE* pData, int pLen);
    
        /** 
            The AES PHM algorithm generates key from the password 
            Note: use this method only if the pass phrase is a plain text,
            otherwise use rather init(BYTE*, int);
        */
        //void init(const CString& pPassword);
    
    
        void init(BYTE* pBuffer, int pLength);
    
    
    
    }; //class
    
    } //namespace
    
} //namespace crypt


#endif //SIAESENCDEC_H_INCLUDED