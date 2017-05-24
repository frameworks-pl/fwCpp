#ifndef SIDBTOOLKIT_H_INCLUDED
#define SIDBTOOLKIT_H_INCLUDED


namespace fw
{
	namespace db
	{

		class SQLObject;
		class SQLObjectDataSet;
		class MaxIDDataSet;
		class SQLT3_Connection;

		//class with some usefull tools related to data storing/loading
		class SIDB_DLLEXPORT DBToolkit
		{
		public:

			//stores/loads singleton from a database (therefore it assumes that there is only
			//one item in the database table when loading)
			//returns true if the object was actually updated (loaded/saved)
			static bool updateSingleObject(SQLObject* pSQLObject, bool bSave, MaxIDDataSet* pMaxIDDS, SQLT3_Connection* pConn);

			//builds a sequence of delete queries for specified IDs in the specified table
			static CString buildDeleteQueries(const CString& sTableName, const std::vector<SQL_ID>& oIDList);

		}; //class







	}; //namespace 
}


#endif //SIDBTOOLKIT_H_INCLUDED