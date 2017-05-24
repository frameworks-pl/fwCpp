#ifndef SIDBPI_H_INCLUDED
#define SIDBPI_H_INCLUDED


//a master header with all classed
//provided in the sidb library/dll

#ifdef _DEBUG
#define ALLOW_PERFORMANCE_ISSUES
#endif //_DEBUG

#include "db.h"

#include "BLOBCollection.h"
#include "dbexception.h"
#include "sqlrow.h"
#include "sqlobject.h"
#include "sqlt3_connection.h"
#include "sqlobject.h"
#include "SQLObjectDataSet.h"
#include "SQLRowDataSet.h"
#include "MaxID.h"
#include "MaxIDDataSet.h"
#include "SQLParam.h"
#include "SQLXMLDataItem.h"
#include "dbtoolkit.h"

//extern void SIDB_DLLEXPORT SIDBSetLogPointer(fw::debug::Logger* pLog);


#endif //SIDBPI_H_INCLUDED
