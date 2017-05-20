#include "stdafx.h"
#include "sicryptexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sicrypt {

SICryptException::SICryptException(CryptoPP::Exception::ErrorType pErrorType, const std::string& pMsg)
: CryptoPP::Exception(pErrorType, pMsg) {

}; //constructor


}; //namespace