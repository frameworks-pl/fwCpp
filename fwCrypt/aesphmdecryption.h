#ifndef AESPHMDECRYPTION_H_INCLUDED
#define AESPHMDECRYPTION_H_INCLUDED


namespace sicrypt {

/**
  @brief This class has been produced based on example
  in Crypto++ User's Guide (CryptoPPGuide.chm).
  It is AES (PHM) Decryption class that uses 256-bit long
  key (produced from passphrase)
*/

class AES_PHM_Decryption : public CryptoPP::Filter  {

 public:

     AES_PHM_Decryption(byte const* pbKey, unsigned int nKeyLen, CryptoPP::BufferedTransformation* pOutQueue = 0);

     void Put(byte const* pbData, unsigned int nDataLen);
     void MessageEnd(int nPropagation = -1);

    //the methods below are fakes (I had to implement them because they are pure virtual in base class)
    unsigned int Put2(const byte *inString, unsigned int length, int messageEnd, bool blocking) { return 0; }
    bool IsolatedFlush(bool hardFlush, bool blocking) { return false; }

 private:
     CryptoPP::SecByteBlock m_sbbKey;
     CryptoPP::SecByteBlock m_sbbCiphertext;
     unsigned int m_nCiphertextBufSize;
     unsigned int m_nCiphertextLen;
 };


}; //namespace



#endif //AESPHMDECRYPTION_H_INCLUDED