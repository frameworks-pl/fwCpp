#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include "fwfile.h"

namespace fw
{
	namespace core
	{

		class Files
		{
			public:
				virtual ~Files() { }
				void addFile(const File& pFile);

				//returns index of file in All collection
				int nameToIndex(const CString& pFileName);

				//returns all directories and files as one vector (directories first)
				const std::vector<File>& getAll() const;

				//returns only files
				const std::vector<File>& getFiles() const;

				//returns only directories
				const std::vector<File>& getDirectories() const;

			protected:
				std::vector<File> m_Files;
				std::vector<File> m_Directories;
				mutable std::vector<File> m_All;
		};

	}
}

#endif // !FILES_H_INCLUDED
