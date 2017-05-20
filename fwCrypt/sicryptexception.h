#ifndef SICRYPTEXCEPTION_H_INCLUDED
#define SICRYPTEXCEPTION_H_INCLUDED

#include <string>
#include <crypto5/cryptlib.h>


namespace sicrypt {


/** 
  @brief Base class for exceptions thrown by this library 
*/
  
class SICryptException : public CryptoPP::Exception {

  public:
    SICryptException(CryptoPP::Exception::ErrorType pErrorType, const std::string& pMsg);


}; //class

}; //namespace


#endif //SICRYPTEXCEPTION_H_INCLUDED