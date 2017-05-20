#ifndef SISHA256_H_INCLUDED
#define SISHA256_H_INCLUDED

#include <crypto5/cryptlib.h>
#include <src/fw/core/fwbytebuffer.h>


namespace sicrypt {

/**
  @brief This class can be used to either
  generate sha256 hash from simple password
  or verify that the password gives the same hash (is correct(
*/

class SISHA256 {

  public:

    SISHA256();
    ~SISHA256();
    /**
      @brief Generates SHA256 from provided passphrase 
    */
    //void create(const CString& pPassPhrase);

    /**
      @brief Generates SHA256 from the provided buffer of data 
    */
    void create(BYTE* pData, int pLength);

    /**
      @brief Initializes the hash from the provided buffer.
      Important: if the size of the hash too short or too long
      the method throws exception!!!
    */
    void init(BYTE* pHash, int pHashLen);



    /** Copy constructor */
    SISHA256(const SISHA256& pSHA256);

    SISHA256& operator=(const SISHA256& pSHA256);
    bool operator==(const SISHA256& pSHA256);
    bool operator!=(const SISHA256& pSHA256) { return !operator==(pSHA256); }

    /**
      @brief Verifies if the password is correct,
      this function will throw an exception if the
      hash havent been initialized.
    */
    bool verify(const fw::core::ByteBuffer& pPassPhrase);

    /**
      @brief Static wrapper returning size of the hash
    */
    static int getHashSize();


    /**
      @brief Returns pointer to the beginning of the hash
      throws exception if the buffor is not initilized.
    */
    BYTE* getBuffer();

  protected:

    /**
      @brief Buffer to store that hash
    */
    fw::core::ByteBuffer mHash;



}; //class


}; //namespace



#endif //SISHA256_H_INCLUDED