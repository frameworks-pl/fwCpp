#include "stdafx.h"
#include "aesphmdecryption.h"
#include "fwcryptexception.h"


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

void 
AES_PHM_AppendToBuffer(CryptoPP::SecByteBlock& sbb, unsigned int& nBufSize, unsigned int& nTotalLen, byte const* pbData, unsigned int nDataLen) {

  if (nTotalLen + nDataLen > nBufSize)
  {
      nBufSize = 2*(nTotalLen + nDataLen);
      sbb.Grow(nBufSize);
  }
  memcpy(sbb+nTotalLen, pbData, nDataLen);
  nTotalLen += nDataLen;

}; //AES_PHM_AppendToBuffer


void AES_PHM_GenerateIvFromSeed(byte const* pbIvSeed, byte* pbIv) {

  CryptoPP::SecByteBlock sbbIvHash(CryptoPP::SHA256::DIGESTSIZE);
  CryptoPP::SHA256().CalculateDigest(sbbIvHash, pbIvSeed, IV_SEED_SIZE);

  // Assuming that SHA256::DIGESTSIZE == 32, IV_SEED_SIZE == 8
  for (unsigned int i=0; i!=IV_SIZE; ++i)
      pbIv[i] = sbbIvHash[i] ^ sbbIvHash[i+16];
} //AES_PHM_GenerateIvFromSeed


namespace fw
{

    namespace crypt {
    
    AES_PHM_Decryption::AES_PHM_Decryption(byte const* pbKey, unsigned int nKeyLen, CryptoPP::BufferedTransformation* pOutQueue)
    : Filter(pOutQueue), m_nCiphertextBufSize(0), m_nCiphertextLen(0)  {
    
        m_sbbKey.resize(CryptoPP::SHA256::DIGESTSIZE);
        CryptoPP::SHA256().CalculateDigest(m_sbbKey, pbKey, nKeyLen);
    
     } //constructor
    
    
    void 
    AES_PHM_Decryption::Put(byte const* pbData, unsigned int nDataLen)  {
    
        AES_PHM_AppendToBuffer(m_sbbCiphertext, m_nCiphertextBufSize, m_nCiphertextLen, pbData, nDataLen);
    
    }; //Put
    
    
    void 
    AES_PHM_Decryption::MessageEnd(int nPropagation) {
    
      if (m_nCiphertextLen < MINIMUM_CIPHERTEXT_LENGTH) {
        CryptException ex(CryptoPP::Exception::OTHER_ERROR, "AES PHM Decryption: Invalid Ciphertext or Key");
        throw ex;
      }
    
    
      // Make the code less cluttered by using the following values
      unsigned int nMacPosition = m_nCiphertextLen - CryptoPP::HMAC<CryptoPP::SHA>::DIGESTSIZE;
      unsigned int nIvSeedPosition = nMacPosition - IV_SEED_SIZE;
    
      // Verify MAC
      if (!CryptoPP::HMAC<CryptoPP::SHA>(m_sbbKey, CryptoPP::SHA256::DIGESTSIZE).VerifyDigest(
              m_sbbCiphertext + nMacPosition,
              m_sbbCiphertext, nMacPosition)) {
        CryptException ex(CryptoPP::Exception::OTHER_ERROR, "AES PHM Decryption: Invalid Ciphertext or Key");
        throw ex;      
      }
    
    
      // Generate IV from seed
      CryptoPP::SecByteBlock sbbIv(IV_SIZE);
      AES_PHM_GenerateIvFromSeed(m_sbbCiphertext + nIvSeedPosition, sbbIv);
    
      // Initialize AES
      CryptoPP::AESEncryption aes(m_sbbKey, CryptoPP::SHA256::DIGESTSIZE);
      CryptoPP::CFBDecryption decryptor(aes, sbbIv);
    
      // Decrypt
      decryptor.ProcessString(m_sbbCiphertext, nIvSeedPosition);
    
      // See how much padding we have
      unsigned int nRandomPaddingLen = (m_sbbCiphertext[0] & 0x0f) + 3;
    
      // Output plaintext
      if (AttachedTransformation())
          AttachedTransformation()->Put(m_sbbCiphertext + nRandomPaddingLen, nIvSeedPosition - nRandomPaddingLen);
    
      m_sbbCiphertext.resize(0);
      m_nCiphertextLen = 0;
      m_nCiphertextBufSize = 0;
    
      CryptoPP::Filter::MessageEnd(nPropagation);
    
     }; //MessageEnd
    
    } //namespace crypt

} //namespace fw