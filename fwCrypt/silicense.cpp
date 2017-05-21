#include "stdafx.h"
#include "silicense.h"

#include <src/fw/core/fwdatetime.h>
#include "sisha256.h"
#include "siencdec.h"
#include "siaesencdec.h"
#include "sicryptexception.h"
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwtextconv.h>

#include <tinyxml.h>

#include <afxdisp.h> //COleDateTime

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw {

    namespace crypt {
    
    
    const std::string License::mLicPassword = "ZDGMSBHD50003";
    const CString License::mLicDateTimeMask = _T("Y-m-d H:i");
    
    License::License() {
    
      mFreeMode = true;
      mLicenseState = LIC_UNINITIALIZED;
    
    }; //default constructor
    
    
    License::License(const CString& pName, const CString& pEmail, 
                         const CString& pAppIdent, bool pFreeMode,
                         const SYSTEMTIME& pFullLicenseLimit) {
    
      mName = pName;
      mEmail = pEmail;
      mFreeMode = pFreeMode;
      mAppIdentifier = pAppIdent;
      if (fw::core::DateTime::isValid(pFullLicenseLimit))
        mFullLicenseExpirationDateTime = fw::core::DateTime::timeToString(mLicDateTimeMask, pFullLicenseLimit);
    
    
      generate();
    
    
    }; //constructor
    
    
    
    
    
    void
    License::generate() {
      
      try {
    
        //name must not be empty
        if (mName.IsEmpty()) {
          TRACE(_T("SILicense::generate: name is missing.\n"));
          mLicenseState = LIC_BAD;
          return;
        };
    
        if (mAppIdentifier.IsEmpty()) {
          TRACE(_T("SILicense::generate: application identifier is missing.\n"));
          mLicenseState = LIC_BAD;
          return;
        }
    
        //setting version of the library that is being used
        //to generate the license
        mVersion = getLibVersion();
    
        //setting license generation date/time
        SYSTEMTIME now = fw::core::DateTime::getCurrentDateTime();
        mLicenseGenerationDateTime = fw::core::DateTime::timeToString(mLicDateTimeMask, now);
    
        //copying all data to one buffer, that is used as the pass phrase
        //to generate checksum (sha256)
        fw::core::ByteBuffer buffer = createPassPhrase();
    
        //calculating checksum
        SHA256 sha256;
        sha256.create(buffer.getBuffer(), buffer.getLength());
    
    #ifdef _DEBUG
    #if (0)
        CFile f;
        if (f.Open(_T("h:\\temp\\test1.bin"), CFile::modeWrite | CFile::modeCreate)) {
          f.Write(buffer.getBuffer(), buffer.getLength());
          f.Close();
        }
    #endif
    #endif
    
        
        fw::core::ByteBuffer sha256_encoded_hex = EncDec::hexEncode(sha256.getBuffer(), CryptoPP::SHA256::DIGESTSIZE);
    
        CString checksum((char*)sha256_encoded_hex.getBuffer(), sha256_encoded_hex.getLength());
    
        //generating license XML
        TiXmlElement el(fw::core::TextConv::Unicode2UTF8(_T("license")));
        TiXmlElement xml_name(fw::core::TextConv::Unicode2UTF8(_T("name")));
        xml_name.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(mName)));
        el.InsertEndChild(xml_name);
    
        TiXmlElement xml_email(fw::core::TextConv::Unicode2UTF8(_T("email")));
        xml_email.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(mEmail)));
        el.InsertEndChild(xml_email);
    
        TiXmlElement xml_gentime(fw::core::TextConv::Unicode2UTF8(_T("generation_time")));
        xml_gentime.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(mLicenseGenerationDateTime)));
        el.InsertEndChild(xml_gentime);
    
        TiXmlElement xml_exptime(fw::core::TextConv::Unicode2UTF8(_T("expiration_time")));
        xml_exptime.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(mFullLicenseExpirationDateTime)));
        el.InsertEndChild(xml_exptime);
    
        CString fmode;
        fmode.Format(_T("%d"), mFreeMode ? 1 : 0);
        TiXmlElement xml_freemode(fw::core::TextConv::Unicode2UTF8(_T("free_mode")));
        xml_freemode.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(fmode)));
        el.InsertEndChild(xml_freemode);
    
        TiXmlElement xml_libversion(fw::core::TextConv::Unicode2UTF8(_T("lib_version")));
        xml_libversion.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(getLibVersion())));
        el.InsertEndChild(xml_libversion);
    
        TiXmlElement xml_appidentifier(fw::core::TextConv::Unicode2UTF8(_T("app_ident")));
        xml_appidentifier.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(mAppIdentifier)));
        el.InsertEndChild(xml_appidentifier);
    
        TiXmlElement xml_checksum(fw::core::TextConv::Unicode2UTF8(_T("checksum")));
        xml_checksum.InsertEndChild(TiXmlText(fw::core::TextConv::Unicode2UTF8(checksum)));
        el.InsertEndChild(xml_checksum);
    
        std::string std_license_full_xml;
        std_license_full_xml << el;
       // CString license_full_xml(UTF82Unicode(std_license_full_xml.c_str()));
    
        //encrypting the license
        //1. creating SHA256 from empty string
        SHA256 xml_sha256;
        xml_sha256.create((BYTE*)mLicPassword.c_str(), (int)mLicPassword.size());
    
        AESEncDec aes;
        aes.init(xml_sha256.getBuffer(), xml_sha256.getHashSize());
        fw::core::ByteBuffer xml_encrypted  = aes.encrypt((BYTE*)std_license_full_xml.c_str(), (int)std_license_full_xml.size());
        fw::core::ByteBuffer xml_encrypted_encoded = EncDec::hexEncode(xml_encrypted.getBuffer(), xml_encrypted.getLength());
    
        mRawLicense = std::string((const char*)xml_encrypted_encoded.getBuffer(), xml_encrypted_encoded.getLength());
    
      }
      catch (crypt::CryptException) 
      {
        mLicenseState = LIC_BAD;
      }
      catch (CryptoPP::Exception) 
      {
        mLicenseState = LIC_BAD;
      }
    
      mLicenseState = LIC_OK;
    
    }; //generate
    
    
    void
    License::read() {
    
      try {
    
        //first of all decode license from hex
        fw::core::ByteBuffer decoded_lic = EncDec::hexDecode((BYTE*)mRawLicense.c_str(), (int)mRawLicense.size());
        SHA256 sha256;
        sha256.create((BYTE*)mLicPassword.c_str(), (int)mLicPassword.size());
        AESEncDec aes;
        aes.init(sha256.getBuffer(), sha256.getHashSize());
    
        fw::core::ByteBuffer decrypted_lic = aes.decrypt(decoded_lic.getBuffer(), decoded_lic.getLength());
    
        //raw xml ready
        std::string license_xml((const char*)decrypted_lic.getBuffer(), decrypted_lic.getLength());
    
        //parsing XML and extracting subsequent values!!
        TiXmlDocument doc;    
        doc.Parse(license_xml.c_str(), 0, TIXML_ENCODING_UTF8);
    
        if (doc.Error()) {
          mLastError = CString(doc.ErrorDesc());
          TRACE(_T("ERROR: SILicense::read, %s\n"), mLastError);
          mLicenseState = LIC_BAD;
          return;
        } 
        TiXmlElement* pRoot = doc.RootElement();
        if (!pRoot) {
          mLastError = _T("License XML parsing failed!");
          mLicenseState = LIC_BAD;
          return;
        }
    
        //making sure that the root element's name is "license"
        CString root_name = fw::core::TextConv::UTF82Unicode(pRoot->Value());
        if (root_name.CompareNoCase(_T("license"))) {
          mLastError = _T("License XML is corrupted.");
          mLicenseState = LIC_BAD;
          return;
        }
    
        TiXmlElement* pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("name")));
        if (!pElement) {
          mLastError = _T("Name/company not found.");
          mLicenseState = LIC_BAD;
          return;
        } 
        CString s(fw::core::TextConv::UTF82Unicode(pElement->GetText()));
        if (s.IsEmpty()) {
          mLastError = _T("Name/company is empty.");
          TRACE(_T("SILicense::read: name is empty\n"));
          mLicenseState = LIC_BAD;
          return;
        }
        mName = s;
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("email")));
        if (!pElement) {
          mLastError = _T("Email not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        mEmail = fw::core::TextConv::UTF82Unicode(pElement->GetText());
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("generation_time")));
        if (!pElement) {
          mLastError = _T("License generation time not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        mLicenseGenerationDateTime = fw::core::TextConv::UTF82Unicode(pElement->GetText());
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("expiration_time")));
        if (!pElement) {
          mLastError = _T("Full version exipration date/time not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        mFullLicenseExpirationDateTime = fw::core::TextConv::UTF82Unicode(pElement->GetText());
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("free_mode")));
        if (!pElement) {
          mLastError = _T("License type not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        CString sfree_mode(fw::core::TextConv::UTF82Unicode(pElement->GetText()));
        mFreeMode = !sfree_mode.CompareNoCase(_T("1"));
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("app_ident")));
        if (!pElement) {
          mLastError = _T("Application identifier not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        mAppIdentifier = fw::core::TextConv::UTF82Unicode(pElement->GetText());
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("lib_version")));
        if (!pElement) {
          mLastError = _T("License library version not found.");
          mLicenseState = LIC_BAD;
          return;
        }
        mVersion = fw::core::TextConv::UTF82Unicode(pElement->GetText());
    
        pElement = pRoot->FirstChildElement(fw::core::TextConv::Unicode2UTF8(_T("checksum")));
        if (!pElement) {
          mLastError = _T("License checksum not found.");
          mLicenseState = LIC_BAD;
          return;
        }
    
        std::string checksum(pElement->GetText());
    
        //verifying if the checksum is ok
        //1. decrypting original checksum 
        fw::core::ByteBuffer checksum_decrypted = EncDec::hexDecode((BYTE*)checksum.c_str(), (int)checksum.size());
        //2. initiating sha256 with the decrypted checksum
        SHA256 checksum_sha256;
        checksum_sha256.init(checksum_decrypted.getBuffer(), checksum_decrypted.getLength());
    
        //3. createing pass phrase to be verified
        if (!checksum_sha256.verify(createPassPhrase())) {
          mLastError = _T("The license is corrupted or it is not a license file.");
          mLicenseState = LIC_BAD;
          return;
        }
    
      }
      catch (crypt::CryptException& ex) {
        mLastError = ex.what();
        TRACE(_T("ERROR: SILicense::read: %s"), mLastError);
        mLicenseState = LIC_BAD;
        return;
    
      }
      catch (CryptoPP::Exception& ex) {
        mLastError = ex.what();
        TRACE(_T("ERROR: SILicense::read: %s"), mLastError);
        mLicenseState = LIC_BAD;
        return;
      }
    
      mLastError = _T("");
      mLicenseState = LIC_OK;
    
    }; //read
    
    
    bool
    License::save(const CString& pFilePath) {
    
      if (mLicenseState == LIC_OK) {
    
        CFile file;
        CFileException ex;
        if (file.Open(pFilePath, CFile::modeCreate | CFile::modeWrite, &ex)) {
          file.Write(mRawLicense.c_str(), (int)mRawLicense.size());
          file.Close();
        }
        else {
          TRACE(_T("ERROR: SILicense::save: %s"), fw::core::AppUtils::getLastError());
          return false;
        }
        return true;
        
      }
    
      return false;
    
    }; //save
    
    
    bool
    License::load(const CString& pFilePath) {
    
      CFile file;
      CFileException ex;
      if (file.Open(pFilePath, CFile::modeRead, &ex)) {
    
        clear();
    
        fw::core::ByteBuffer pBuffer((int)file.GetLength());
        file.Read(pBuffer.getBuffer(), pBuffer.getLength());
        file.Close();
        mRawLicense = std::string((const char*)pBuffer.getBuffer(), pBuffer.getLength());
    
        //license have been loaded from file
        //trying to read it!
        read(); 
    
        if (!isOk())
          return false;
    
      }
      else {
        TRACE(_T("SILicense::load: %s"), fw::core::AppUtils::getLastError());
        return false;
      }
    
      return true;
    
    }; //save
    
    
    
    fw::core::ByteBuffer 
    License::createPassPhrase() {
    
      std::string name = fw::core::TextConv::Unicode2UTF8(mName);
      std::string email = fw::core::TextConv::Unicode2UTF8(mEmail);
      std::string gentime = fw::core::TextConv::Unicode2UTF8(mLicenseGenerationDateTime);
      std::string exptime = fw::core::TextConv::Unicode2UTF8(mFullLicenseExpirationDateTime);
    
      std::string ver = fw::core::TextConv::Unicode2UTF8(getVersion());
      std::string app_ident = fw::core::TextConv::Unicode2UTF8(mAppIdentifier);
    
      //preparing buffer for the pass phrase that will be 
      int len = (int)(name.size() + email.size() + 1 + gentime.size() + exptime.size() + ver.size() + app_ident.size()); //the one is for the free mode
      
      fw::core::ByteBuffer buffer(len);
      BYTE* offset = buffer.getBuffer();
    
      //coppying name to the buffer
      memcpy(offset, name.c_str(), name.size());
      offset += name.size();
    
      //coppying email to the buffer
      if (email.size()) {
        memcpy(offset, email.c_str(), email.size());
        offset += email.size(); 
      }
    
      //coppying free mode to the buffer
      BYTE free_mode = (mFreeMode) ? 1 : 0;
      memcpy(offset, &free_mode, 1);
      offset +=1;
    
      //copying generation date time (it is alwyas set, so we do not need to check it's length) 
      memcpy(offset, gentime.c_str(), gentime.size());
      offset += gentime.size();
    
      //coppying full license expiration date/time
      if (exptime.size())  {
        memcpy(offset, exptime.c_str(), exptime.size());
        offset += exptime.size();
      }
    
      //copying library version used to generate the license
      memcpy(offset, ver.c_str(), ver.size());
      offset += ver.size();
    
      memcpy(offset, app_ident.c_str(), app_ident.size());
      offset += app_ident.size();
    
    
      return buffer;
    
    }; //createPassPhrase
    
    
    SYSTEMTIME 
    License::getLicenseGenerationTime() {
    
    
      SYSTEMTIME st;
      memset(&st, 0, sizeof(SYSTEMTIME));
      if (!mLicenseGenerationDateTime.IsEmpty()) {
        fw::core::DateTime::stringToTime(mLicenseGenerationDateTime, mLicDateTimeMask, &st);
      }
    
      return st;
    
    
    
    }; //getLicenseGenerationTime
    
    
    SYSTEMTIME 
    License::getFullLicenseTimeLimitation() const {
    
      SYSTEMTIME st;
      memset(&st, 0, sizeof(SYSTEMTIME));
      if (!mFullLicenseExpirationDateTime.IsEmpty())
        fw::core::DateTime::stringToTime(mFullLicenseExpirationDateTime, mLicDateTimeMask, &st);
    
      return st;
    
    }; //getFullLicenseTimeLimitation
    
    
    const CString& 
    License::getLibVersion() { 
      
      static CString lib_version(_T("2.0"));
      return lib_version;
    
    }; //getLibVersion
    
    
    void
    License::clear() {
    
       mAppIdentifier.Empty();
       mEmail.Empty();
       mFreeMode = true;
       mFullLicenseExpirationDateTime.Empty();
       mLicenseGenerationDateTime.Empty();
       mName.Empty();
    
       mRawLicense.clear();
       mVersion.Empty();
    
       mLicenseState = LIC_UNINITIALIZED;
    
    }; //clear
    
    
    
    bool 
    License::parse(const std::string& pLicRaw) {
    
      clear();
    
      mRawLicense = pLicRaw;
      read();
    
      return isOk();
    
    
    }; //parse
    
    
    int 
    License::countDaysPassed(const SYSTEMTIME& pStartDate) {
    
    	COleDateTime now = COleDateTime::GetCurrentTime();
    	COleDateTime reg_time(pStartDate);
    	COleDateTimeSpan time_span = now - reg_time;
    
    	double dDays = time_span.GetTotalDays();
    	if (dDays < 0) dDays = -dDays;
    
      return (int)dDays;
    
    
    
    }; //countDaysLeft
    
    
    bool 
    License::isFullLicense() const {
    
      if (!isOk())
        return false;
    
      if (mFreeMode)
        return false;
    
      SYSTEMTIME fullic_limit = getFullLicenseTimeLimitation();
      if (fw::core::DateTime::isValid(fullic_limit)) {
        SYSTEMTIME now = fw::core::DateTime::getCurrentDateTime();
        //if the full license date/time is less than now
        //it means the license has expired
        if (fw::core::DateTime::compareTimes(fullic_limit, now) == fw::core::DateTime::FIRST_LESS_THAN_SECOND)
          return false;
      }
    
      return true;
    
    }; //isFullLicense
    
    
    bool 
    License::verifyAppIdent(const CString& pAppIdent) {
    
      if (!isOk())
        return false;
    
      return !mAppIdentifier.CompareNoCase(pAppIdent);
    
    }; //verifyAppIdent
    

    } //namespace crypt
    
} //namespace fw