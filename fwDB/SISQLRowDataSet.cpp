#include "stdafx.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sidb
{

SISQLRowDataSet::SISQLRowDataSet(const CString& sReadQuery)
{
    m_sReadQuery = sReadQuery;

}


void SISQLRowDataSet::append(const SISQLRow& pRow)
{
  m_RowList.push_back(pRow);

}


}; //namespace