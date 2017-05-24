#ifndef SISQLOBJECT_H_INCLUDED
#define SISQLOBJECT_H_INCLUDED

#include "SQLParam.h"

namespace fw
{
	namespace db
	{

		class SQLObjectDataSet;
		class SQLT3_Connection;


		//an abstract class of an entity that
		//knows how to serialize itself from/to SQL database
		class SIDB_DLLEXPORT SQLObject : public CObject
		{


			friend class SQLObjectDataSet;
			friend class SQLT3_Connection;


		public:

			//one of the states in which the SQL object can be
			enum StateEnum
			{
				STATE_NEW = 1,
				STATE_OK_LOADED, //the object has been just loaded from a database
				STATE_OK,
				STATE_DELETE,
				STATE_UPDATE
			};

			//initializes all static stuctures of the SISQLObject class
			static void init();

			//returns string representation of the object's state
			static CString stateToString(int iState);

			static const CString m_sSQLID_FieldName;

			SQLObject();
			virtual ~SQLObject() { }

			//need to define custom copy constructor due to overriding CObject
			//(see C2558 for details)
			SQLObject(const SQLObject& pObject);

			//need to define custom copy constructor due to overriding CObject
			//(see C2558 for details)
			const SQLObject& operator=(const SQLObject& pObject);

			//initializes itself from the external copy of an object
			virtual bool initialize(const SQLObject* pObject);

			//initializes itself after loading/saving
			virtual bool initialize() = 0;

			//returns unique SQL identifier of the object
			const SQL_ID& getSQLID() const { return m_SQLID; }

			//sets object as deleted
			void setDeleted();
			bool isDeleted() const;

			//true if object has just been loaded from database
			bool isLoaded() const;

			//returns true if object has one of states that require update
			bool needsUpdate() const;


			//force the object to be UPDATED
			virtual void setUpdated();

			//force the object to be NEW with invalid SQLID
			virtual void setNew();
			bool isNew() const;

			//get state in which the object is
			StateEnum getState() const { return m_eState; }

			//adds new param to the object
			void addParam(const SQLParam& pParam);

			//removes param identified by the column name
			//returns true if the param has been removed as expected
			bool removeParam(const CString& pName);

			//prepares the insert/update/delete query for execution
			//the returned value is number of variables to be bound
			virtual int getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery);

			//returns the n-th param to be bound to the compiled sql query
			//if there is no param with such index, returns false
			const SQLParam& getParamToBind(int iIndex);


			void updateFromBLOB(const CString& pColName, const fw::core::ByteBuffer& pBLOB);
			void updateFromInt(const CString& pColName, int iValue);
			void updateFromString(const CString& pColName, const CString sValue);


		protected:


			//sets arbitratry state of the object    
			void setState(StateEnum pState) { m_eState = pState; }

			//returns table name to which this object is bound
			virtual CString getTableName() = 0;

			//sets identifier for the object (this method should be called only once perf lifetime of the object)
			void setSQLID(const SQL_ID& pSQLID);

			//serializes the object into a query that can be run on SQL database
			CString serialize();


			//state of the object
			StateEnum m_eState;

			//the identifier of this object in database (the sql_id)
			SQL_ID m_SQLID;

			//collection of values (params)
			ParamMap m_ParamMap;

			//map of params that should be bound when constructing a query
			//this map is initialized by each call to getUpdateQuery
			ParamVarsMap m_ObjectsToBind;

			//maps object states into strings
			static std::map<int, CString> m_StateToStringMap;




		};


		typedef std::list<SQLObject*> SQLObjectList;
		typedef std::map<SQL_ID, SQLObject*> SQLObjectMap;
	}; //namespace 
}



#endif //SISQLOBJECT_H_INCLUDED