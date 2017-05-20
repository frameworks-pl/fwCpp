#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sidb 
{

SIDBException::SIDBException(const CString& sMsg)
: fw::core::Exception(sMsg)
{
    m_iDBErrorCode = -1;

}


SIDBException::SIDBException(sqlite3* pDBHandle)
: fw::core::Exception(_T(""))
{
  if (pDBHandle)
  {
    m_iDBErrorCode = sqlite3_errcode(pDBHandle);
    const char* pErrorMsg = sqlite3_errmsg(pDBHandle);
    mMessage = fw::core::TextConv::UTF82Unicode(pErrorMsg);
  }


}



}; //namespace