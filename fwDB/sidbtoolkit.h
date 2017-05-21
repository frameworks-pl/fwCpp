#ifndef SIDBTOOLKIT_H_INCLUDED
#define SIDBTOOLKIT_H_INCLUDED


namespace fw
{
	namespace db
	{

		class SISQLObject;
		class SISQLObjectDataSet;
		class SIMaxIDDataSet;
		class SI_SQLT3_Connection;

		//class with some usefull tools related to data storing/loading
		class SIDB_DLLEXPORT SIDBToolkit
		{
		public:

			//stores/loads singleton from a database (therefore it assumes that there is only
			//one item in the database table when loading)
			//returns true if the object was actually updated (loaded/saved)
			static bool updateSingleObject(SISQLObject* pSQLObject, bool bSave, SIMaxIDDataSet* pMaxIDDS, SI_SQLT3_Connection* pConn);

			//builds a sequence of delete queries for specified IDs in the specified table
			static CString buildDeleteQueries(const CString& sTableName, const std::vector<SQL_ID>& oIDList);

		}; //class







	}; //namespace 
}


#endif //SIDBTOOLKIT_H_INCLUDED