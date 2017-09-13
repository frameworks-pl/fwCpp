#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

namespace fw
{
	namespace core
	{
		class File
		{
			public:

				static SYSTEMTIME INVALID_SYSTEM_TIME;

				File()  : m_bDirectory(false), m_Size(0)
				{ 
				}

				File(const CString& sPath, bool bDirectory = false, DWORD dwSize = 0, SYSTEMTIME updateDate = INVALID_SYSTEM_TIME)
					: m_sPath(sPath), m_bDirectory(bDirectory), m_Size(dwSize), m_LastUpdated(updateDate)
				{ 

				}
				virtual ~File() { }

				const CString& getPath() const { return m_sPath; }
				CString getName() const;
				DWORD getSize() const { return m_Size;  }
				CString getSizeAsString() const;
				CString getLastUpdateDateAsString() const;

				bool isDirectory() const { return m_bDirectory;  }

			protected:

				//Full path
				CString m_sPath;

				//is directory
				bool m_bDirectory;

				//file size LARGE_INTEGER
				DWORD m_Size;

				//last update date
				SYSTEMTIME m_LastUpdated;
		};


		class FileSorter
		{
			enum Columns
			{
				eName = 0x1,
				eDirectory = 0x2
			};

		};

	}
}




#endif // !FILE_H_INCLUDED
