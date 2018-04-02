#ifndef SISQLPARAM_H_INCLUDED
#define SISQLPARAM_H_INCLUDED

#include <list>
#include <fwxmlnode.h>
#include <src/main/fwCommon.h>

#include <src/fw/core/fwexception.h>
#include <src/fw/core/fwgenericnotifier.h>
#include <src/fw/core/fwbytebuffer.h>
#include "dbpi.h"

namespace fw
{
	namespace db
	{

		class FWDB_DLLEXPORT SQLParam
		{
		public:

			enum ParamTypeEnum
			{
				PARAM_UNKNOWN = 0,
				PARAM_INTEGER = 1,
				PARAM_STRING,
				PARAM_BLOB,
				PARAM_BLOB_POINTER
			};


			//constructs integer sql param
			SQLParam(const CString& pColName, int* pIntValue);

			//constructs string sql param
			SQLParam(const CString& pColName, CString* pStringValue);

			//constructs blob sql param
			SQLParam(const CString& pColName, fw::core::ByteBuffer* pByteBuffer);

			//constructs blob pointer sql param
			SQLParam(const CString& pColName, BLOBItem* pBLOBItem);

			virtual ~SQLParam() { }

			const CString& getColumnName() const { return m_sColumnName; }

			//puts formatted value into pFormattedValue, returns true
			//if the value is actually a param to be bound
			virtual bool getSQLFormattedValue(CString& pFormattedValue) const;

			static const SQLParam& invalid();
			bool isValid() const;

			//returns pointer to buffer of a blob param, throws if this
			//the method was called not for the BLOB
			const fw::core::ByteBuffer& getBLOBBuffer() const;

			//initializes the param from provided blob, throws exception if this is not a BLOB param
			void updateFromBLOB(const fw::core::ByteBuffer& pBLOB);

			//initializes the param from provided int, throws exception if this is not an Int param
			void updateFromInt(int pIntValue);

			//initializes the param from provided string, throws exception if this is not a string param
			virtual void updateFromString(const std::string& pStringValue);

			//makes copy of itself and resturns it as a new object
			virtual SQLParam* clone() const;


		protected:


			SQLParam();
			void init();


			//Type of the value sotred in the param
			ParamTypeEnum m_eParamType;

			int* m_pIntValue;
			CString* m_pStringValue;
			fw::core::ByteBuffer* m_pByteBuffer;
			db::BLOBItem* m_pBLOBItem;

			//name of this param in sqlite database
			CString m_sColumnName;





		}; //class

		typedef std::map<CString /*Column name*/, SQLParam*> ParamMap;
		typedef std::map<int, SQLParam*> ParamVarsMap;



	}; //namespace 
}



#endif //SISQLPARAM_H_INCLUDED