#ifndef SILICENSE_H_INCLUDED
#define SILICENSE_H_INCLUDED

#include <src/fw/core/fwbytebuffer.h>


namespace fw {

    namespace crypt {
    
    
    /**
    This class has two purposes:
    1. Take user data like name, e-mail, etc. and generate the checksum, then
       license xml, and then encrypts and encodes it
    2. Take license string, decrypt it and extract data or report an error
    Any errors will be reported by error codes, and critical errors will
    be reported by throwing an exception.
    */
    
    class License {
    
      public:
        enum LicenseStateEnum {
          LIC_OK,
          LIC_UNINITIALIZED, //license is uninitialized (this is the default state after construction)
          LIC_BAD //the license is bad (cannot be generated because of some data is missing, checksume failed, etc)
        };
        typedef LicenseStateEnum LicenseState;
    
        /** @brief The default constructor */
        License();
    
        /** @brief The constructor that takes all the data */
        License(const CString& pName, const CString& pEmail, const CString& pAppIdent, bool pFreeMode,
          const SYSTEMTIME& pFullLicenseLimit);
    
        /**
          @brief Dummy password used to encrypt/decrypt license xml
        */
        static const std::string mLicPassword;
    
        /**
           @brief The date/time mask used inside the license
        */
        static const CString mLicDateTimeMask;
    
        /**
          @brief Loads license from the file, after you use this method
          check if license is ok (LIC_OK) before you start reading values
        */
        bool load(const CString& pFilePath);
    
        /**
          @brief This method is similar to the load, but it takes
          the license in the raw form (encoded + hex encrypted)
          and tries to read it
        */
        bool parse(const std::string& pLicRaw);
    
    
        /**
          @brief Stores license to the file but only if its state 
          is LIC_OK
        */
        bool save(const CString& pFilePath);
    
        /** @brief Returns true if the license have satus LIC_OK
            (it has been generated with success or it has been
            read and validated with success)
        */
        bool isOk() const { return mLicenseState == LIC_OK; }
    
        /**
          @brief Returns license generation time as SYSTEMTIME
        */
        SYSTEMTIME getLicenseGenerationTime();
    
        /** @brief Returns company/user name for whom the license
            was generated
        */
        const CString& getName() const { return mName; }
    
        /**
          @brief Returns the full license time limitation.
          Before apply limitiations make sure that date is valid!!
        */
        SYSTEMTIME getFullLicenseTimeLimitation() const;
    
        /** Returns the e-mail related to name/company for which
            the license was generated
        */
        const CString& getEmail() const { return mEmail; }
    
        /** @brief Returns the application identifier
            for which the license was generated
        */
        const CString& getAppIdent() const { return mAppIdentifier; }
    
        /** @brief Returns the version of the library used
            to generate the license (you may compare it with the 
            value return by getVersion and do something based
            on the result!
        */
        const CString& getVersion() const { return mVersion; }
    
        /** @brief Returns version of the license library */
        static const CString& getLibVersion();
    
        /**
          @brief Returns the last error that that stopped
          the license reading/parsing
        */
        const CString& getLastError() const { return mLastError; }
    
    
        /**
          @brief Returns number of days that have passed since pStartDate
        */
        static int countDaysPassed(const SYSTEMTIME& pStartDate);
    
        /**
          @brief Returns true if the application mFreeMode is off (0)
          and full license limitation time is not set
          or it is set but it still before the current date/time
        */
        bool isFullLicense() const;
    
        /** @brief Returns true if the provided application identifier
            matches the one in the license.
        */
        bool verifyAppIdent(const CString& pAppIdent);
    
    
      protected:
    
        /**
          @brief Creates the buffer containing the pass phrase
          used to generate the checksum (or to verify if the checksum 
          it ok).
        */
        fw::core::ByteBuffer createPassPhrase();
    
        /**
          @brief Takes all data, validates it and generates the checksum
          from it. In case something is wrong, puts the license into
          LIC_BAD state, otherwise puts the license into LIC_OK state.
        */
        void generate();
    
    
        /** 
            @brief Parses the license assuming that its raw form (ecrypted, hex encoded)
            is placed in the mRawLicense.
            Puts the mLicenseState in the appropriate mode. 
        */
        void read();
    
        /**
          @brief Clears all fields of the control (this is required
          to properly verify license after loading!)
        */
        void clear();
        
    
        /**
          @brief Holds the current state of the license
        */
        LicenseState mLicenseState;
    
        /** @brief Name/surname or company name (must not be empty) */
        CString mName;
    
        /** @brief Email of the person/company */
        CString mEmail;
    
        /** @brief If this flag is one, the application is in the free mode
            that is "design tool" is not available
        */
        int mFreeMode;
    
        /** @brief Identifier of the application for which the license
            was generated (it should be compared with it when
            license is being checked in the program).
    
            It also must not be empty!
        */
        CString mAppIdentifier;
    
        /** @brief Version of the library used to generate the license */
        CString mVersion;
    
        /** 
          @brief The time when the license was generated 
          This date/time is always stored in the following format Y-m-d H:i (i. e. 2006-07-29 10:28)
        */
    
        CString mLicenseGenerationDateTime;
    
    
        /** @brief The time when the full licens expires
            ignored if empty.
            The license owner decides how to store date/time (what format to use)
            This date/time is always stored in the following format Y-m-d H:i (i.e. 2006-07-29 19:23)
        */
        CString mFullLicenseExpirationDateTime;
    
    
        /**
          @brief The generated and encoded license string (in UTF8)
        */
        std::string mRawLicense;
    
        /**
          @brief The last error that stopped the license parsing
          It can be taken and shown for diagnostics
        */
        CString mLastError;
    
    
    
    
    
    }; //class
    
    
    } //namespace
    
} //namespace fw


#endif //SILICENSE_H_INCLUDED