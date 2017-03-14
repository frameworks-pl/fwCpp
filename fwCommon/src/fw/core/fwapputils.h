#ifndef APPUTILS_H_INCLUDED
#define APPUTILS_H_INCLUDED


namespace fw {

    namespace core 
    {


    //enum to express possible measurement systems
    enum MeasurementSystemEnum
    {
      MS_METRIC = 1,
      MS_US     = 2
    
    }; 
    
    class AppUtils 
    {
    
      public:
    
            /**
            @brief Returns path to the directory from which
            the application has been launched (without the backslash at the end)
            */
            static CString getAppPath();
    
    
            /**
            @brief Returns filename (with extension) of the app executable
            */
            static CString getExecutableFileName();
    
    
            /**
                @brief Returns path to the application data folder for the current user
                or empty string if the path could not be retrieved
            */
            static CString getAppDataFolder();
    
    
            /**
            @brief Returns the GetLastError as CString */
            static CString getLastError();
    
            /**
                @brief Returns random string of the specified length.
                The string contains only alphanumeric characters.
            */
            static CString getRandomString(int pLength);
    
            /**
                @brief Generates a GUID as string
            */
            static CString getGUIDAsString();
    
            /**
                This is kind of complicated method to check if the them is currently
                used in the application.
                Even though there is a method IsThemeActive, it works for the entire
                system, not for the application it is called from.
                So to checkout if theme is on, we first check if the OS version is 
                5.1 or later (XP or later).
                If it is, we create a dummy control (button) and take its parent (creator)
                handle, then we take handle to comctl32.dll.
                If both handles are true the theme is on, otherwise it is off.
    
                @brief pWnd - a window needed to create a dummy control
            */
            static bool isThemeOn(CWnd* pWnd);
    
            //gets information about OS version
            static bool getOSVersion(int& iMajorVersion, int& iMinorVersion);
    
    
    
            /**
    	        Returns value of the specified environment variable
    	        or empty string if the variable was not found
            */
            static CString getEnvVar(const CString& pVarName);
    
            //returns value of an environment variable as integer
            //if variable does not exists or its value is NaN the returned value is zero
            static int getEnvVarAsInt(const CString& pVarName);
    
    
            /**
            Tries to get a path to default browser,
            if succeeds returns true
            */
            static bool getPathToDefaultBrowser(CString& pPathToBrowser);
    
            //returns true if two CObjects represent the same runtime class
            static bool runtimeObjectsHaveTheSameClass(CObject* pFirstObj, CObject* pSecondObj);
    
    
            //returns decimal point used in the system (first tries from user settings,
            //then from system settings)
            static TCHAR getDecimalPoint();
    
            //returns measurement system used in the system
            static MeasurementSystemEnum getMeasurementSystem();
    
            //returns a path to the temporary folder
            static CString getTempFolderPath();
    
            //returns true if there is an active internet connection
            static bool isInternetActive();
    
        protected:
    
            //remembers if xp themes are available
            static bool m_bThemesAreOn;
    
            //remembers if checking of themes availability was done
            static bool m_bThemesChecked;
    
    }; //class

    } //namespace core

} //namespace fw

#endif //APPUTILS_H_INCLUDED