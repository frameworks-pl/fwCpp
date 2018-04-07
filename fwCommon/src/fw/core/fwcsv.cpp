#include "fwCommon_pch.h"
#include "fwcsv.h"
#include <fstream>

namespace fw
{
	namespace core
	{

		std::vector<CString> CSV::m_EmptyVector;

		void CSV::Load(const CString& pFilePath)
		{
			m_Content.clear();

			FILE* fStream;
			errno_t errCode = _tfopen_s(&fStream, pFilePath, _T("r, ccs=UNICODE"));
			if (!errCode)
			{
				CStdioFile File(fStream);
				CString sLine;				
				while (File.ReadString(sLine))
				{
					m_sResultFileContent.Append(sLine);
					int iCurPos = 0;
					CString sCurrToken = sLine.Tokenize(CString(m_cSeparator), iCurPos);
					std::vector<CString> csvValuesOneLine;
					while (sCurrToken != _T(""))
					{
						csvValuesOneLine.push_back(sCurrToken);
						sCurrToken = sLine.Tokenize(CString(m_cSeparator), iCurPos);						
					}

					m_Content.push_back(csvValuesOneLine);
				}
			}
			fclose(fStream);
		}


		void CSV::fromString(const CString& csv)
		{
			

		}


		int CSV::ColumnNameToIndex(const CString& pColumnName) const
		{
			if (m_bHasHeader && m_Content.empty() == false)
			{
				std::vector<CString>::const_iterator it;
				int iIndex = 0;
				for (it = m_Content[0].begin(); it != m_Content[0].end(); it++, iIndex++)
				{
					if (*it == pColumnName)
						return iIndex;
				}
			}

			return -1;
		}

		const std::vector<CString>& CSV::CellValueToRow(const CString& pColumnName, const CString& pValue) const
		{
			int iColIndex = ColumnNameToIndex(pColumnName);
			if (iColIndex != -1)
			{
				std::vector<std::vector<CString>>::const_iterator it;
				for (it = m_Content.begin(); it != m_Content.end(); it++)
				{
					if (it != m_Content.begin() || !m_bHasHeader)
					{
						if ((*it)[iColIndex] == pValue)
							return *it;
					}
				}
			}

			return m_EmptyVector;
		}


		CString CSV::GetCellValue(int iRow, int iCol) const
		{
			CString sResult;
			if (iRow < (m_bHasHeader ? m_Content.size() - 1 : m_Content.size()) && iCol < m_Content[iRow].size())
				sResult = m_Content[iRow][iCol];

			return sResult;
		}

	}; //namespace common
}; //namespace jg