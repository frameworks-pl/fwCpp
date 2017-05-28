#ifndef SIMAXIDDATASET_H_INCLUDED
#define SIMAXIDDATASET_H_INCLUDED

#include "SQLObjectDataSet.h"
#include "db.h"

namespace fw
{
	namespace db
	{

		class MaxID;

		class FWDB_DLLEXPORT MaxIDDataSet : public SQLObjectDataSet
		{

		public:
			MaxIDDataSet();
			MaxIDDataSet(CRuntimeClass* pClass);

			SQL_ID set(const SQLObject* pSQLObject);

			//returns next unique identifier for the object specified by name
			Identifier getNextID(const CString& pObjectName);

			//returns current max id for the specified object name
			//NOTE: DO NOT use this method to assign next ID, use getNextID instead!!!
			const Identifier& getCurrentMaxID(const CString& pObjectName);

#if defined(_DEBUG) && defined(ALLOW_PERFORMANCE_ISSUES)
			void dumpMaxIDs();
#endif

		protected:

			//finds the object with the specified name
			MaxID* findMaxObject(const CString& pObjectName);

			//this data set must "manually" assign identifier to itslef
			int m_iMaxID;

			//produces new max id object
			virtual MaxID* createNewMaxID(const CString& pObjectName);

		};


	}; //namespace 
}



#endif //SIMAXIDDATASET_H_INCLUDED