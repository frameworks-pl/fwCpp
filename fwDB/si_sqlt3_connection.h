#ifndef SISQLT3CONNECTION_H_INCLUDED
#define SISQLT3CONNECTION_H_INCLUDED


#include "SISQLRowDataSet.h"
#include "SISQLObjectDataSet.h"
#include "SIGenericConnection.h"

namespace sidb
{

class SI_SQLT3_Connection_Listener : public fw::core::GenericListener
{
  public:

    //notifies about change in the count of stored/loaded items
    virtual void onUpdatedItemsCountChange(int iCount) { }

}; 


//the connection to SQLite3 database
class SIDB_DLLEXPORT SI_SQLT3_Connection : public SIGenericConnection, public fw::core::GenericNotifier
{

  public:
    SI_SQLT3_Connection();

    //opens connection to the specified database (failes if database does not exist)
    void openConnection(const CString& pFilePath);

    //opens or creates the specified database and then runs
    //the specified script
    void createDatabase(const CString& pFilePath, const CString& pScript);

    //check if the connection is open
    bool isOpen() const { return (NULL != m_pCnnHandle); }

    //closes connection to the database
    void closeConnection();

    //this is general "read" query function - to return raw rows of data (no write!!!)
    //The query to run is inside the dataset...
    void query(SISQLRowDataSet& pRowDataset);

	//executes raw query without returning any result
	void query(const CString& pQuery);

    //this query can be used to read/write SQL objects    
    //all queries needed to read/write data are taken from the object type inside the dataset
    //returns number of objects saved/loaded (which might be different from number of objects in the dataset!!!)
    int query(SISQLObjectDataSet& pObjectDataset, bool bWrite, const CString& pWherePhrase = _T(""), bool bClearCollection = true);

    //performs read/write operations using the object passed
    //the id is needed when loading an object from database (when writing this parameter is ignored)
    void query(SISQLObject* pObject, bool bWrite, SQL_ID sqlID);

    //returns number of items to load for the specified collection
    int countItemsToLoad(SISQLObjectDataSet& pObjectDataset);

    //get path to database
    const CString& getDatabaseFilePath() const { return m_sPath; }

    //begins a transaction
    void beginTransaction();

    //ends transaction with commit or rollback
    void endTransaction(bool bCommit = true);

    //returns true if a table with the specified name exists
    bool tableExists(const char* pTableName);


  protected:

    //reads a single record
    void readRecord(sqlite3_stmt* pStmt, SISQLObject* pSQLObject);

    //writes single recrod
    void writeRecord(sqlite3_stmt* pStmt, SISQLObject* pSQLObject, int iParamsCount);

    //callback function to collect results of a query (raw rows)
    static int QueryCallbackRowDataSet(void* pParm, int pColCnt, char** pRowVals ,char** pColNames);

    //callback function to collect results of a query (sql objects)
    static int QueryCallbackObjectDataSet(void* pParm, int pColCnt, char** pRowVals ,char** pColNames);

    //string with sqlite3 file
    CString m_sPath;

    //handle to the connection
    sqlite3* m_pCnnHandle;



}; //class


}; //namespace


#endif //SICONNECTION_H_INCLUDED