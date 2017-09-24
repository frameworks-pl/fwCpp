#ifndef SISHA256_H_INCLUDED
#define SISHA256_H_INCLUDED

#include <crypto5/cryptlib.h>
#include <src/fw/core/fwbytebuffer.h>


namespace fw {

    namespace crypt {
    
    
    /**
      @brief This class can be used to either
      generate sha256 hash from simple password
      or verify that the password gives the same hash (is correct(
    */
    
    class SHA256 {
    
      public:
    
        SHA256();
        ~SHA256();
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
        SHA256(const SHA256& pSHA256);
    
        SHA256& operator=(const SHA256& pSHA256);
        bool operator==(const SHA256& pSHA256);
        bool operator!=(const SHA256& pSHA256) { return !operator==(pSHA256); }
    
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

		//return hash as byte buffer
		const fw::core::ByteBuffer& getByteBuffer() const { return mHash;  }

		//returns SHA256 as base64 encoded string
		CString toBase64();

		//creates sha256 from provided base64 string
		void fromBase64(const CString& sBase64String);

		//encodes buffer to base64
		static CString b64encode(const void* data, const size_t len);

		//decodes from base64
		static fw::core::ByteBuffer b64decode(const CString& sBase64String);

    
      protected:
    
        /**
          @brief Buffer to store that hash
        */
        fw::core::ByteBuffer mHash;
    
    
    
    }; //class
    
    
    } //namespace
    
} //namespace fw



#endif //SISHA256_H_INCLUDED