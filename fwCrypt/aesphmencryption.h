#ifndef AESPHMENCRYPTION_H_INCLUDED
#define AESPHMENCRYPTION_H_INCLUDED


#pragma warning (push)
#pragma warning (disable: 4231)
#include <crypto5/filters.h>
#pragma warning (pop)


namespace fw
{

    namespace crypt {
    
    
    /**
      @brief This class has been produced based on example
      in Crypto++ User's Guide (CryptoPPGuide.chm).
      It is AES (PHM) Encryption class that uses 256-bit long
      key (produced from passphrase)
    */
    
    class AES_PHM_Encryption : public CryptoPP::Filter {
    
      public:
    
        AES_PHM_Encryption(CryptoPP::RandomNumberGenerator& rng, byte const* pbKey, unsigned int nKeyLen,
          CryptoPP::BufferedTransformation* pOutQueue = 0);
        void Put(byte const* pbData, unsigned int nDataLen);
        void MessageEnd(int nPropagation = -1);
    
        //the methods below are fakes (I had to implement them because they are pure virtual in base class)
        unsigned int Put2(const byte *inString, unsigned int length, int messageEnd, bool blocking) { return 0; }
        bool IsolatedFlush(bool hardFlush, bool blocking) { return false; }
    
    
      private:
         CryptoPP::RandomNumberGenerator& m_rng;
         CryptoPP::SecByteBlock m_sbbKey;
         CryptoPP::SecByteBlock m_sbbPlaintext;
         unsigned int m_nPlaintextBufSize;
         unsigned int m_nPlaintextLen;
    
    
    }; //class
    
    } //namespace crypt
    
} //namespace fw


#endif //AESPHMENCRYPTION_H_INCLUDED