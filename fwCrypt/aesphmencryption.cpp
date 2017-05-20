#include "stdafx.h"
#include "aesphmencryption.h"


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace sicrypt {

AES_PHM_Encryption::AES_PHM_Encryption(CryptoPP::RandomNumberGenerator& rng, byte const* pbKey, unsigned int nKeyLen,
                         CryptoPP::BufferedTransformation* pOutQueue) : CryptoPP::Filter(pOutQueue), m_rng(rng), m_nPlaintextBufSize(0), m_nPlaintextLen(0) {

     //creating 256-bit long key from pass phrase
     m_sbbKey.resize(CryptoPP::SHA256::DIGESTSIZE);                           
     CryptoPP::SHA256().CalculateDigest(m_sbbKey, pbKey, nKeyLen);

 }; //constructor



 void 
 AES_PHM_Encryption::Put(byte const* pbData, unsigned int nDataLen) {

  AES_PHM_AppendToBuffer(m_sbbPlaintext, m_nPlaintextBufSize, m_nPlaintextLen, pbData, nDataLen);

 }; //Put


 void 
AES_PHM_Encryption::MessageEnd(int nPropagation) {

  CryptoPP::SecByteBlock sbbPadding(18);

    // We want the plaintext length to be a multiple of 15 bytes
    unsigned int nRandomPaddingLen = 15 - (m_nPlaintextLen%15);
    byte byFirst = m_rng.GenerateByte();
    sbbPadding[0] = ((byFirst ^ (byFirst << 4)) & 0xf0) | (nRandomPaddingLen & 0x0f);
    nRandomPaddingLen += 3;
    m_rng.GenerateBlock(sbbPadding+1, nRandomPaddingLen-1);

    // Generate IV seed
    CryptoPP::SecByteBlock sbbIvSeed(IV_SEED_SIZE);
    m_rng.GenerateBlock(sbbIvSeed, IV_SEED_SIZE);

    // Generate IV from seed
    CryptoPP::SecByteBlock sbbIv(IV_SIZE);
    AES_PHM_GenerateIvFromSeed(sbbIvSeed, sbbIv);

    // Initialize AES
    CryptoPP::AESEncryption aes(m_sbbKey, CryptoPP::SHA256::DIGESTSIZE);
    CryptoPP::CFBEncryption encryptor(aes, sbbIv);

    // Encrypt
    encryptor.ProcessString(sbbPadding, nRandomPaddingLen);
    encryptor.ProcessString(m_sbbPlaintext, m_nPlaintextLen);

    // Calculate MAC over encrypted plaintext and IV seed
    CryptoPP::SecByteBlock sbbMac(CryptoPP::HMAC<CryptoPP::SHA>::DIGESTSIZE);
    CryptoPP::HMAC<CryptoPP::SHA> mac(m_sbbKey, CryptoPP::SHA256::DIGESTSIZE);
    mac.Update(sbbPadding, nRandomPaddingLen);		// encrypted
    mac.Update(m_sbbPlaintext, m_nPlaintextLen);	// encrypted
    mac.Update(sbbIvSeed, IV_SEED_SIZE);			// plain
    mac.Final(sbbMac);

    if (AttachedTransformation())  {

          AttachedTransformation()->Put(sbbPadding, nRandomPaddingLen);
          AttachedTransformation()->Put(m_sbbPlaintext, m_nPlaintextLen);
          AttachedTransformation()->Put(sbbIvSeed, IV_SEED_SIZE);
          AttachedTransformation()->Put(sbbMac, CryptoPP::HMAC<CryptoPP::SHA>::DIGESTSIZE);
    }

     m_sbbPlaintext.resize(0);
     m_nPlaintextLen = 0;
     m_nPlaintextBufSize = 0;

     CryptoPP::Filter::MessageEnd(nPropagation);

}; //MessageEnd







}; //namespace