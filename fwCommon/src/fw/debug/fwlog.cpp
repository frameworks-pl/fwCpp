#include "fwCommon_pch.h"
#include "fwlog.h"

namespace fw
{
    namespace debug
    {
        
        Logger* Logger::m_pLogger = NULL;

        Logger::Logger()
        {
        }

        Logger::~Logger()
        {
            std::map<std::string, FILE*>::iterator it;
            for (it = m_FilePathToFileHandle.begin(); it != m_FilePathToFileHandle.end(); it++)
            {
                if (it->second)
                {
                    fflush(it->second);
                    fclose(it->second);
                    it->second = NULL;
                }
            }

            m_FilePathToFileHandle.clear();
            m_LevelToLevelStructMap.clear();           
        }

        Logger* Logger::Get()
        {
            if (m_pLogger == NULL)
            {
                m_pLogger = new Logger();
                //m_pLogger->IncludeLogging(LogType::eEventPropagation);
            }

            return m_pLogger;
        }


        void Logger::Destroy()
        {
            delete m_pLogger;
            m_pLogger = NULL;
        }

        void Logger::AddLogLevel(int iLogLevel, const char* sFilePath, const char* sLevelName)
        {
            Logger::Get()->InternalAddLogLevel(iLogLevel, sFilePath, sLevelName);
        }

		void Logger::RemoveLogLevel(int iLogLevel, const char* sFilePath)
		{
			Logger::Get()->InternalRemoveLogLevel(iLogLevel, sFilePath);
		}

		void Logger::RemoveAllLogLevels(const char* sFilePath)
		{
			Logger::Get()->InternalRemoveAllLogLevels(sFilePath);
		}

        void Logger::SetLogDateTime(int iLogLevel, bool bLogTime, bool bLogDate)
        {
            Logger::Get()->InternalSetLogDateTime(iLogLevel, bLogTime, bLogDate);
        }

        void Logger::SetLogLevel(int iLogLevel, bool bLogLevel)
        {
            Logger::Get()->InternalSetLogLevel(iLogLevel, bLogLevel);
        }

		void Logger::vLog(int iLogLevel, const char* pLogValue, va_list ap)
		{

			//figure out how big buffer we are going to need #ifdef WIN32
#ifndef UNIX
			int n = _vscprintf(pLogValue, ap);
#else
			int n = 1024; //snprintf(NULL, 0, pLogValue, ap);
#endif
			//add space for trailing zero
			n += 2;

			//allocate the buffer
			char* pBuffer = new char[n];
			memset(pBuffer, 0, n);

			//now format the string
			//int nsize = vsprintf(pBuffer, pLogValue, args); #ifdef WIN32
#ifndef UNIX
			_vsnprintf(pBuffer, n, pLogValue, ap);
#else
			vsnprintf(pBuffer, n, pLogValue, ap);
#endif

			std::string sFormattedValue(pBuffer);
			delete[] pBuffer;
			pBuffer = NULL;

			Logger::Get()->InternalLog(iLogLevel, sFormattedValue);
		}


        void Logger::InternalAddLogLevel(int iLogLevel, const char* sFilePath, const char* sLevelName)
        {
            if (m_LevelToLevelStructMap.find(iLogLevel) != m_LevelToLevelStructMap.end())
            {
                throw std::string("Log level already added");
            }

            LogLevel logLevel;
            logLevel.m_sFilePath = sFilePath;
            logLevel.m_sName = sLevelName;

            //create file handle if needed
            FILE* pFile = NULL;
            if (m_FilePathToFileHandle.find(sFilePath) == m_FilePathToFileHandle.end())
            {
                pFile = fopen(sFilePath, "w+");
                if (pFile)
                    m_FilePathToFileHandle.insert(std::map<std::string, FILE*>::value_type(sFilePath, pFile));
            }
            else
                pFile = m_FilePathToFileHandle[sFilePath];

            if (pFile)
                m_LevelToLevelStructMap.insert(std::map<int, LogLevel>::value_type(iLogLevel, logLevel));
        }

		void Logger::InternalRemoveLogLevel(int iLogLevel, const char* sFilePath)
		{
			std::map<int, LogLevel>::iterator levelIt = m_LevelToLevelStructMap.find(iLogLevel);
			if (levelIt == m_LevelToLevelStructMap.end())
			{
				throw std::string("Log level does not exist");
			}

			//remove that level from map
			m_LevelToLevelStructMap.erase(levelIt);

			//let's check if there are any other targets with that file path
			std::string stdFilePathOfRemovedLevel(sFilePath);
			for (levelIt = m_LevelToLevelStructMap.begin(); levelIt != m_LevelToLevelStructMap.end(); levelIt++)
			{
				if (levelIt->second.m_sFilePath == stdFilePathOfRemovedLevel)
					break;
			}

			if (levelIt == m_LevelToLevelStructMap.end())
			{
				//no more levels with that path

				//find proper file handle
				std::map<std::string, FILE*>::iterator filepathIt =  m_FilePathToFileHandle.find(sFilePath);
				if (filepathIt != m_FilePathToFileHandle.end())
				{
					//flush changes and close it
					fflush(filepathIt->second);
					fclose(filepathIt->second);

					//end finally remove from map
					m_FilePathToFileHandle.erase(filepathIt);
				}
			}

		}

		void Logger::InternalRemoveAllLogLevels(const char* sFilePath)
		{
			std::vector<int> levels_to_remove;
			std::string stdFilePathOfRemovedLevels(sFilePath);

			//build list of targets that are directed to given file
			std::map<int, LogLevel>::iterator levelIt;
			for (levelIt = m_LevelToLevelStructMap.begin(); levelIt != m_LevelToLevelStructMap.end(); levelIt++)
			{
				if (levelIt->second.m_sFilePath == stdFilePathOfRemovedLevels)
					levels_to_remove.push_back(levelIt->first);
			}

			//remove each level
			std::vector<int>::const_iterator it;
			for (it = levels_to_remove.begin(); it != levels_to_remove.end(); it++)
			{
				InternalRemoveLogLevel(*it, sFilePath);
			}

		}

        void Logger::InternalSetLogDateTime(int iLogLevel, bool bLogDate, bool bLogTime)
        {
            std::map<int, LogLevel>::iterator it = m_LevelToLevelStructMap.find(iLogLevel);
            if (it == m_LevelToLevelStructMap.end())
            {
                throw std::string("Log level not found");
            }
            it->second.m_bIncludeDate = bLogDate;
            it->second.m_bIncludeTime = bLogTime;
        }

        void Logger::InternalSetLogLevel(int iLogLevel, bool bLogLevel)
        {
            std::map<int, LogLevel>::iterator it = m_LevelToLevelStructMap.find(iLogLevel);
            if (it == m_LevelToLevelStructMap.end())
            {
                throw std::string("Log level not found");
            }
            it->second.m_bIncludeLevelName = bLogLevel;
        }

        void Logger::Log(int iLogLevel, const char* pLogValue, ...) 
        {
             va_list args;
             va_start( args, pLogValue );

             //figure out how big buffer we are going to need #ifdef WIN32
            #ifndef UNIX
                 int n = _vscprintf(pLogValue, args); 
            #else
				int n = 1024; //snprintf(NULL, 0, pLogValue, args); 
            #endif

             //add space for trailing zero
             n += 2;

             //allocate the buffer
             char* pBuffer = new char[n];
             memset(pBuffer, 0, n);

             //now format the string
             //int nsize = vsprintf(pBuffer, pLogValue, args); #ifdef WIN32
            #ifndef UNIX
             _vsnprintf(pBuffer, n, pLogValue, args); 
            #else
             vsnprintf(pBuffer, n, pLogValue, args); 
            #endif
             va_end( args );

             std::string sFormattedValue(pBuffer);
             delete[] pBuffer;
             pBuffer = NULL;

             Logger::Get()->InternalLog(iLogLevel, sFormattedValue);
         }

        void Logger::InternalLog(int iLevel, const std::string& pValue)
        {
            std::map<int, LogLevel>::const_iterator level_it = m_LevelToLevelStructMap.find(iLevel);

            if (level_it != m_LevelToLevelStructMap.end())
            {
                //find file handle for this level
                std::map<std::string, FILE*>::iterator file_it = m_FilePathToFileHandle.find(level_it->second.m_sFilePath);
                if (file_it != m_FilePathToFileHandle.end())
                {
                    std::string sRawValue(pValue);
                    Decorate(sRawValue, level_it->second.m_sName, level_it->second.m_bIncludeTime, level_it->second.m_bIncludeDate, level_it->second.m_bIncludeLevelName);
                    fwrite((const void*)sRawValue.c_str(), sizeof(char), sRawValue.length(), file_it->second);
                    fflush(file_it->second);
                }
            }
        }

        void Logger::Decorate(std::string& pRawString, const std::string& pLogLevel, bool bIncludeTime, bool bIncludeDate, bool bIncludeLevel)
        {
          std::string sResult;
          if (bIncludeTime || bIncludeDate || bIncludeLevel)
          {
              sResult.append("[");
              std::string sDateTime = GetDateTime(bIncludeDate, bIncludeTime);
              if (false == sDateTime.empty())
              {
                sResult.append(sDateTime);
                sResult.append(" ");
              }

              if (bIncludeLevel)
                sResult.append(pLogLevel);

              sResult.append("] ");
              sResult.append(pRawString);
              StripBreaks(sResult);

              pRawString = sResult;
          }
        }

        void Logger::StripBreaks(std::string& pString)
        {

          //first strip breaks from the end of the line
          while (false == pString.empty())
          {
              char c = pString[pString.size()-1];
              if (c == 10 || c == 13)
                pString.erase(pString.size()-1);
              else
                break;
          }

          //add one single break;
          pString.append("\n");

        }

        std::string Logger::GetDateTime(bool bDate, bool bTime)
        {
            std::string sResult;

            const int iSize = 64;
            char szTime[iSize];
            char szDate[iSize];

            time_t rawtime;    
            time(&rawtime);
            tm* timeinfo = localtime(&rawtime);

            if (true == bTime)
            {
              strftime(szTime, iSize, "%H:%M", timeinfo);
              sResult.append(std::string(szTime));
            }

            if (false == sResult.empty())
              sResult.append(" ");

            if (true == bDate)
            {
              strftime(szDate, iSize, "%d-%m-%Y", timeinfo);
              sResult.append(std::string(szDate));
            }

            return sResult;
            
        }        


    }; //debug

}; //et