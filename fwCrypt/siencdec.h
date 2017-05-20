#ifndef SIENCDEC_H_INCLUDED
#define SIENCDEC_H_INCLUDED

#include <src/fw/core/fwbytebuffer.h>

namespace sicrypt {

/**
  The base class for all encryption/decryption classes.
  It holds two keys (public/private) though the given algorithm
  may not use one or both of them.
  SIEncDec ensures freeing key on destruction (the key(s) should
  be initialized by the 'init' method written for each class.
  The init method should also set the length of the used key.
*/

class SIEncDec  {

  public:
    SIEncDec();
    ~SIEncDec();

    /**
      @brief Encodes the specified buffer to hex,
      The caller is responsible for freeing the returned
      buffer.
    */
    static fw::core::ByteBuffer hexEncode(BYTE* pData, UINT pLen);

    /**
      @brief Decodes the specified buffer from hex
      The caller is responsible for freeing the returned
      buffer.
    */
    static fw::core::ByteBuffer hexDecode(BYTE* pData, UINT pLen);

    /**
      @brief Encodes/Decodes the specified buffer by XORing each byte
      with the provided mask (byte)
    */
    static void xor(BYTE* pData, UINT pLen, BYTE pMask);

	/**
	  @brief Encodes/Decodes the specified buffer by XORing each byte
	  with previous one (starting from the second one)
    */
	static void xorMoving(BYTE* pData, UINT pLen);

    /**
      @brief Performs encryption on the specified buffer of data.
   */
    virtual fw::core::ByteBuffer encrypt(const BYTE* pData, int pLen) = 0;

    /**
      @brief Performs decryption of the specified buffer of data.
    */
    virtual fw::core::ByteBuffer decrypt(const BYTE* pData, int pLen) = 0;

  protected:

    /** @brief Buffer holding public key. The init method
        is responsible for allocating space for the key and copying it*/
    BYTE* mPublicKey;

    /** @brief The length of the public key */
    int mPublicKeyLen;

    /** Buffer holding private key. The init method is responsible
        for allocating space for the key and copying it*/
    BYTE* mPrivateKey;

    /** @brief The length of the private key */
    int mPrivateKeyLen;

}; //class


}; //namespace


#endif //ENCDEC_H_INCLUDED