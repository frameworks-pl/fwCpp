#ifndef SISQLPARAM_H_INCLUDED
#define SISQLPARAM_H_INCLUDED

namespace fw
{
	namespace db
	{

		class SIDB_DLLEXPORT SISQLParam
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
			SISQLParam(const CString& pColName, int* pIntValue);

			//constructs string sql param
			SISQLParam(const CString& pColName, CString* pStringValue);

			//constructs blob sql param
			SISQLParam(const CString& pColName, fw::core::ByteBuffer* pByteBuffer);

			//constructs blob pointer sql param
			SISQLParam(const CString& pColName, SIBLOBItem* pBLOBItem);

			const CString& getColumnName() const { return m_sColumnName; }

			//puts formatted value into pFormattedValue, returns true
			//if the value is actually a param to be bound
			bool getSQLFormattedValue(CString& pFormattedValue) const;

			static const SISQLParam& invalid();
			bool isValid() const;

			//returns pointer to buffer of a blob param, throws if this
			//the method was called not for the BLOB
			const fw::core::ByteBuffer& getBLOBBuffer() const;

			//initializes the param from provided blob, throws exception if this is not a BLOB param
			void updateFromBLOB(const fw::core::ByteBuffer& pBLOB);

			//initializes the param from provided int, throws exception if this is not an Int param
			void updateFromInt(int pIntValue);

			//initializes the param from provided string, throws exception if this is not a string param
			void updateFromString(const CString& pStringValue);



		protected:


			SISQLParam();
			void init();


			//Type of the value sotred in the param
			ParamTypeEnum m_eParamType;

			int* m_pIntValue;
			CString* m_pStringValue;
			fw::core::ByteBuffer* m_pByteBuffer;
			db::SIBLOBItem* m_pBLOBItem;

			//name of this param in sqlite database
			CString m_sColumnName;





		}; //class

		typedef std::map<CString /*Column name*/, SISQLParam> ParamMap;
		typedef std::map<int, SISQLParam> ParamVarsMap;



	}; //namespace 
}



#endif //SISQLPARAM_H_INCLUDED