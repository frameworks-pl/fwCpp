#ifndef FWVERSION_H_INCLUDED
#define FWVERSION_H_INCLUDED


namespace fw 
{
    namespace core
    {
        /**
          @brief This class is a wraper to GetFileVersionInfo function
          (helps to get version information about the specified file)
        */
        
        class Version 
        {
        
          public:
            /** 
              @brief Constructor taking path to the file which information 
              should be retrieved.
              If not specified the name of the calling executable is used
            */
            Version(const CString& pFilePath = NULL);
        
            int getProductMajor() const { return mProductMajor; }
            int getProductMinor() const { return mProductMinor; }
            int getProductRevision() const { return mProductRevision; }
            int getProductBuildNo() const { return mProductBuildNo; }
        
        	//returns major version of the file 
        	int getFileMajor() const { return mFileVerMajor; }
        
        	//returns minor version of the file
        	int getFileMinor() const { return mFileVerMinor; }
        
            CString getProductVersionString(bool bWithBuild = false);
        
          protected:
        
            /** @brief Parses the specified path and extracts version parts */
            void parse(const CString& pFilePath);
        
        
            /** @brief The product's major version */
            int mProductMajor;
        
            /** @brief The product;s minor version */
            int mProductMinor;
        
            /** @brief the procut's revision */
            int mProductRevision;
        
            /** @brief Build number */
            int mProductBuildNo;
        
        
        	int mFileVerMajor;
        	int mFileVerMinor;
        
        }; //class
        
    } //namespace core


} //namespace


#endif //FWVERSION_H_INCLUDED