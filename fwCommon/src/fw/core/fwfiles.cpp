#include "fwCommon_pch.h"
#include "fwfiles.h"

namespace fw
{
	namespace core
	{
		void Files::addFile(const File& pFile)
		{
			if (pFile.isDirectory())
				m_Directories.push_back(pFile);
			else
				m_Files.push_back(pFile);

			m_All.clear();
		}

		const std::vector<File>& Files::getAll() const
		{
			if (this->m_All.empty())
			{
				m_All.reserve(m_Directories.size() + m_Files.size());
				m_All.insert(m_All.end(), this->m_Directories.begin(), m_Directories.end());
				m_All.insert(m_All.end(), this->m_Files.begin(), m_Files.end());
			}
			
			return m_All;

		}

		const std::vector<File>& Files::getFiles() const
		{
			return m_Files;
		}


		const std::vector<File>& Files::getDirectories() const
		{
			return m_Directories;
		}

	}

}