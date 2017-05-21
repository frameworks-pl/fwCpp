#include "stdafx.h"
#include "sicryptexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw
{

    namespace crypt {
    
    CryptException::CryptException(CryptoPP::Exception::ErrorType pErrorType, const std::string& pMsg)
    : CryptoPP::Exception(pErrorType, pMsg) {
    
    }; //constructor
    
    
    } //namespace crypt

} //namespace fw