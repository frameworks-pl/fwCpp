#ifndef SICRYPTEXCEPTION_H_INCLUDED
#define SICRYPTEXCEPTION_H_INCLUDED

#include <string>
#include <crypto5/cryptlib.h>


namespace fw
{


    namespace crypt {
    
    
    /** 
      @brief Base class for exceptions thrown by this library 
    */
      
    class CryptException : public CryptoPP::Exception {
    
      public:
        CryptException(CryptoPP::Exception::ErrorType pErrorType, const std::string& pMsg);
    
    
    }; //class
    
    } //namespace crypto
    
} //namespace fw


#endif //SICRYPTEXCEPTION_H_INCLUDED