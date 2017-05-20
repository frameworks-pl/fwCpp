#ifndef SIAESENCDEC_H_INCLUDED
#define SIAESENCDEC_H_INCLUDED

#include "siencdec.h"


namespace sicrypt {

/**
  @brief AES PHM Encyrption/Decryption class 
*/

class SIAESEncDec : public SIEncDec {

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

}; //namespace


#endif //SIAESENCDEC_H_INCLUDED