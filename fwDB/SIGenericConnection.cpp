#include "stdafx.h"
#include "SIGenericConnection.h"
#include "SISQLObjectDataSet.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace sidb
{

SIGenericConnection::SIGenericConnection()
{

}



void SIGenericConnection::destroyDeletedObjects(SISQLObjectDataSet* pSQLObjectDataSet)
{
  ASSERT(pSQLObjectDataSet);

  pSQLObjectDataSet->destroyDeletedObjects();

}





}; //namespace