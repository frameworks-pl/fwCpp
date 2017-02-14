#ifndef FWLOG_H_INCLUDED
#define FWLOG_H_INCLUDED

#include "fwcommon.h"

typedef int LogType;

namespace fw
{
    namespace debug
    {
    
        class Logger
        {
            public:
                static Logger* Get();
                static void Destroy();
                static void Log(int iLogLevel, const char* pLogValue, ...); 
				static void vLog(int iLogLevel, const char* pLogValue, va_list ap);

                static void AddLogLevel(int iLogLevel, const char* sFilePath, const char* sLevelName);
				static void RemoveLogLevel(int iLogLevel, const char* sFilePath);
				static void RemoveAllLogLevels(const char* sFilePath);
                static void SetLogDateTime(int iLogLevel, bool bLogTime, bool bLogDate);
                static void SetLogLevel(int iLogLevel, bool bLogLevel);

            protected:

                struct LogLevel
                {
                    LogLevel() : m_bIncludeDate(true), m_bIncludeTime(true), m_bIncludeLevelName(true) { }

                    std::string m_sFilePath;
                    std::string m_sName;

                    bool m_bIncludeDate;
                    bool m_bIncludeTime;
                    bool m_bIncludeLevelName;
                };

                Logger();
                virtual ~Logger();

                void InternalAddLogLevel(int iLogLevel, const char* sFilePath, const char* sLevelName);
				void InternalRemoveLogLevel(int iLogLevel, const char* sFilePath);
				void InternalRemoveAllLogLevels(const char* sFilePath);
                void InternalSetLogDateTime(int iLogLevel, bool bLogDate, bool bLogTime);
                void InternalSetLogLevel(int iLogLevel, bool bLogLevel);
                void InternalLog(int iLevel, const std::string& pValue);
                void Decorate(std::string& pRawString, const std::string& pLogLevel, bool bIncludeTime, bool bIncludeDate, bool bIncludeLevel);
                void StripBreaks(std::string& pString);
                std::string GetDateTime(bool bDate, bool bTime);

                static Logger* m_pLogger;
                //std::list<LogType> m_ActiveLogs;
                //std::map<LogType, std::string> m_LogTypeToLogTypeNameMap;

                std::map<int, LogLevel> m_LevelToLevelStructMap;
                std::map<std::string, FILE*> m_FilePathToFileHandle;
        };

    }; //namespace debug

}; //namespace

#ifdef LOGGER
    #define LOG_0(iLevel, iFormatString)                                                     fw::debug::Logger::Log(iLevel, iFormatString)
    #define LOG_1(iLevel, iFormatString, param1)                                             fw::debug::Logger::Log(iLevel, iFormatString, param1)
    #define LOG_2(iLevel, iFormatString, param1, param2)                                     fw::debug::Logger::Log(iLevel, iFormatString, param1, param2)
    #define LOG_3(iLevel, iFormatString, param1, param2, param3)                             fw::debug::Logger::Log(iLevel, iFormatString, param1, param2, param3)
    #define LOG_4(iLevel, iFormatString, param1, param2, param3, param4)                     fw::debug::Logger::Log(iLevel, iFormatString, param1, param2, param3, param4)
    #define LOG_5(iLevel, iFormatString, param1, param2, param3, param4, param5)             fw::debug::Logger::Log(iLevel, iFormatString, param1, param2, param3, param4, param5)
    #define LOG_6(iLevel, iFormatString, param1, param2, param3, param4, param5, param6)     fw::debug::Logger::Log(iLevel, iFormatString, param1, param2, param3, param4, param5, param6)

#else
    #define LOG_0(iLevel, iFormatString)
    #define LOG_1(iLevel, iFormatString, param1)
    #define LOG_2(iLevel, iFormatString, param1, param2)
    #define LOG_3(iLevel, iFormatString, param1, param2, param3)
    #define LOG_4(iLevel, iFormatString, param1, param2, param3, param4)
    #define LOG_5(iLevel, iFormatString, param1, param2, param3, param4, param5)
    #define LOG_6(iLevel, iFormatString, param1, param2, param3, param4, param5, param6)
    
    #define EVENT_NAME(type)
#endif

#endif //FWLOG_H_INCLUDED