#ifndef SIDB_H_INCLUDED
#define SIDB_H_INCLUDED


#ifdef FWDB
	#define FWDB_DLLEXPORT _declspec(dllexport)	
#else
	#define FWDB_DLLEXPORT _declspec(dllimport)
#endif



//STL
#include <string>
#include <map>
#include <algorithm>
#include <list>

//sicommon headers
//#include "sicommoninterface.h"
#include "sqlite3.h" //master header for sqlite3

//#include "tinyxml.h"
//#include "sixmlnode.h" //xmlparser


//#include "silog.h"
namespace fw
{
	namespace db
	{
		typedef unsigned int Identifier;
	}

}

//invalid database identifier
#define INVALID_SQL_ID 0

typedef int SQL_ID;

//map "column name" -> "value"
typedef std::map<CString, CString> NameValueMap;


enum SIDBLoggingLevels
{
  LEVEL_SIDB = 1000,
  LEVEL_SIDB_XML = 1001,
  LEVEL_SIDB_MAXID = 1002,
  LEVEL_SIDB_SQLITE = 1003,
  LEVEL_SIDB_PREPARE_FINALIZE = 1004
};

#endif SIDB_H_INCLUDED