#ifndef SIDBEXCEPTION_H_INCLUDED
#define SIDBEXCEPTION_H_INCLUDED


namespace fw
{
	namespace db
	{

		class SIDB_DLLEXPORT SIDBException : public fw::core::Exception
		{

		public:
			SIDBException(const CString& sMsg);
			SIDBException(sqlite3* pDBHandle);

			//returns underling error code from the database
			int getDBErrorCode() const { return m_iDBErrorCode; }

		protected:

			//underling error code from the database (must be above zero)
			int m_iDBErrorCode;




		}; //class 
	}


};


#endif //SIDBEXCEPTION_H_INCLUDED