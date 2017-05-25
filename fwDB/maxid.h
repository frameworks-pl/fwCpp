#ifndef SIMAXID_H_INCLUDED
#define SIMAXID_H_INCLUDED

#include "sqlxmldataitem.h"

namespace fw
{
	namespace db
	{

		class FWDB_DLLEXPORT MaxID : public SQLXMLDataItem
		{

			DECLARE_DYNCREATE(MaxID)

		public:
			MaxID();
			MaxID(const CString& pMaxIDName);

			//use this method to assign different table name for max ids than "max_ids"
			static void initMaxIDS(const CString& pTableName);

			//returns name of the object for which the max value is held
			const CString& getObjectName() const { return m_sName; }

			//returns the next unique identifier
			Identifier getNextID();

			//return current max id used in this object
			//DO NOT USE this method to assign next ids for new objects!!!
			//use getNextID instead.
			Identifier getCurrentMaxID() const;

			bool initialize(const SQLObject* pObject);
			int getUpdateQuery(CString& pQuery, bool bWrite, bool bCountQuery);

#if defined(_DEBUG) && defined(ALLOW_PERFORMANCE_ISSUES)
			void dumpMaxID();
#endif

		protected:

			bool initialize();

			XMLNode toUnicodeXML() const;
			bool fromUnicodeXML(const XMLNode* pXmlElement);

			CString getTableName() { return m_sTableName; }

			//current "unique" identifier
			Identifier m_iUniqueID;

			//name of this max id
			CString m_sName;

			//byte buffer to serialize the object
			fw::core::ByteBuffer m_sql_maxid_data;

			//static variable holding table name for max ids
			//use "initMaxIDS" to set this variable
			static CString m_sTableName;


		};


	}; //namespace 
}


#endif //SIMAXID_H_INCLUDED