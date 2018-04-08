#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED


//Send comments to: janusz@frameworks.pl
namespace fw
{
    namespace core
    {
        class CSV
        {
			public:
				CSV(bool bHasHeader = true, TCHAR cSeparator = _T('\t')) 
					: m_bHasHeader(bHasHeader), m_cSeparator(cSeparator) { }

				//loads CSV data from file
				void Load(const CString& pFilePath);

				//loads CSV data directly from string
				void fromString(const CString& csv);

				//returns index of the column with a specific name
				//or -1 if: 1) there is no such column, 2) CSV was declared as having no header!
				int ColumnNameToIndex(const CString& pColumnName) const;

				bool HasHeader() const { return m_bHasHeader; }

				const std::vector<std::vector<CString>>& GetContent() const { return m_Content; }
				const CString& GetContentAsString() const { return m_sResultFileContent; }

				const std::vector<CString>& CellValueToRow(const CString& pColumnName, const CString& pValue) const;
				CString GetCellValue(int iRow, int iCol) const;

			protected:

				std::vector<CString> parseLine(const CString& sLine);

				static std::vector<CString> m_EmptyVector;

				//CSV file has header
				bool m_bHasHeader;

				//CSV separator character
				TCHAR m_cSeparator;

				//file content as rows and columns
				std::vector<std::vector<CString>> m_Content;

				//file content as raw string
				CString m_sResultFileContent;
        };
        
    } //namespace common

} //namespace jg
#endif //CSV_H_INCLUDED