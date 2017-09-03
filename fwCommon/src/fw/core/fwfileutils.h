#ifndef FILEUTILS_H_INCLUDED
#define FILEUTILS_H_INCLUDED

#include "fwfiles.h"

namespace fw {

	namespace core {


		//enum with all known image types
		enum ImageTypeEnum
		{
			IMG_TYPE_UNKNOWN,
			IMG_TYPE_BMP,
			IMG_TYPE_PNG,
			IMG_TYPE_GIF,
			IMG_TYPE_JPG
		};
        
        typedef ImageTypeEnum ImageType;


		class FileUtils 
		{

			public:
				/** @brief returns true if the specified directory/folder exists */
				static bool dirExists(const CString& pPath);

				/** @brief returns true if the specified file exists and is not a folder! */
				static bool fileExists(const CString& pPath);

				/** @brief Creates a directory. Returns true on success, false otherwise
					In case of failue the second parameter contains reason (if not null)
					*/
				static bool createDir(const CString& pPath, CString* pErrMsg = NULL);

				/** @brief Removes the provided directory. Return true on success, false otherwise
				In case of failure the second parameter contains reason (if no null)
				*/
				static bool removeDir(const CString& pPath, CString* pErrMsg = NULL);

				/** @brief Extracts file name without extenstion from the path */
				static CString extractFileName(const CString& pPath) { return getPathPart(PATH_PART_FNAME, pPath); }

				/** @brief Extracts file name with the dot and extenstion */
				static CString extractFileNameWithExt(const CString & pPath);

				/** @brief Extracts drive name from the path*/
				static CString extractDriveName(const CString& pPath) { return getPathPart(PATH_PART_DRIVE, pPath); }

				/** @brief Extracts directory name from the path*/
				static CString extractDir(const CString& pPath) { return getPathPart(PATH_PART_DIR, pPath); }

				//extracts extension of afile
				static CString extractExtension(const CString& pPath) { return getPathPart(PATH_PART_EXT, pPath); }

				/** @brief Gets the creation date of a file or folder,
					returns true on success or false on error */
				static bool getCreationDate(const CString& pPath, SYSTEMTIME& pCreationDate);

				/** @brief Gets creation, last access and last modification dates of a file */
				static bool getFileDates(const CString& pPath, SYSTEMTIME& pCreationDate, SYSTEMTIME& pLassAccessDate, SYSTEMTIME& pLastModifDate);

				/**
					@brief Returns list of files and/or directories in the specified directory
					@param pSearchMask - mask to search, f.e. c:\temp\*.log or just *.*
					@param bFiles - include files in the list
					@param bDirs - include directories in the list
					*/
				static std::list<CString> getFileDirList(const CString& pSearchMask, bool bFiles, bool bDirs);

				/**
				  This is simple helper function that removes ending backslash if there is one
				  */
				static void stripEndingBackslash(CString& pPath);

				//returns size of the specified file (if bKbs is true, the size is in kBytes)
				static DWORD getFileSize(const CString& pPath);

				//converts size to a string using appropriate unit (B, kB, MB, GB)
				static CString fileSizeToString(DWORD iSize);

				//converts extension into an internal image type
				static ImageType extensionToImageType(const CString& pExtension);

				//converts image type to a string representing its file extension
				static CString imageExtensionToString(const ImageType eImageType);

				//returns drive letters available in the system
				static std::vector<CString> getVolumes();

				//returns list of files in given folder
				//excludes list of names that should be excluded from search on given path
				//if path does not exist return value is empty vector
				static fw::core::Files getFiles(const CString& pPath, const std::set<CString>& excludes);

				//return path without .. inside
				static CString getAbsolutePath(const CString& sPath);

			protected:

				enum PartEnum {
					PATH_PART_DRIVE = 1,
					PATH_PART_DIR,
					PATH_PART_FNAME,
					PATH_PART_EXT
				};
				static CString getPathPart(const PartEnum& pPart, const CString& pPath);

				//helper method used by fileSizeToString
				static CString InsertSeparator(DWORD dwNumber);

		}; //class

	} //namespace core

} //namespace


#endif //FILEUTILS_H_INCLUDED