#ifndef SISQLROWDATASET_H_INCLUDED
#define SISQLROWDATASET_H_INCLUDED



namespace fw
{
	namespace db
	{

		class SIDB_DLLEXPORT SQLRowDataSet
		{
		public:
			SQLRowDataSet(const CString& sReadQuery);

			//adds new row to the collection
			void append(const SQLRow& pRow);

			//the read query
			const CString& getReadQuery() const { return m_sReadQuery; }

			//returns collection of rows
			const SQLRowList& getRowList() const { return m_RowList; }

			//returns true if dataset is empty
			bool isEmpty() const { return m_RowList.empty(); }

		protected:

			//the list of raw rows (the dataset)
			SQLRowList m_RowList;

			//general read query (the SELECT query)
			CString m_sReadQuery;




		}; //class


	}; //namespace 
}



#endif //SISQLROWDATASET_H_INCLUDED