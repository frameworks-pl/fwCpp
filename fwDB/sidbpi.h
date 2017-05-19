#ifndef SIDBPI_H_INCLUDED
#define SIDBPI_H_INCLUDED


//a master header with all classed
//provided in the sidb library/dll

#ifdef _DEBUG
#define ALLOW_PERFORMANCE_ISSUES
#endif //_DEBUG

#include "sidb.h"

#include "SIBLOBCollection.h"
#include "sidbexception.h"
#include "sisqlrow.h"
#include "sisqlobject.h"
#include "si_sqlt3_connection.h"
#include "sisqlobject.h"
#include "SISQLObjectDataSet.h"
#include "SISQLRowDataSet.h"
#include "SIMaxID.h"
#include "SIMaxIDDataSet.h"
#include "SISQLParam.h"
#include "SISQLXMLDataItem.h"
#include "sidbtoolkit.h"

extern void SIDB_DLLEXPORT SIDBSetLogPointer(SILog* pLog);


#endif //SIDBPI_H_INCLUDED
