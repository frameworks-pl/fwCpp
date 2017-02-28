#include "gtest.h"
#include "ettest.h"
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <src/fw/debug/fwlog.h>

#define DEBUG_LEVEL 1
#define LEVEL_GATE_CREATE 2
#define LEVEL_GATE_ADD_PIN 3

#define LEVEL_WITH_DATETIME 3
#define LEVEL_WITHOUT_DATETIME 4
#define LEVEL_LOOP_LOGGING 5

#define LEVEL_LOOP_LOGGING_1 6 
#define LEVEL_LOOP_LOGGING_2 7
#define LEVEL_LOOP_LOGGING_3 8
#define LEVEL_LOOP_LOGGING_4 9
#define LEVEL_LOOP_LOGGING_5 10

#define LEVEL_LOG_AP  11

#define LEVEL_APPEND 12


//TODO Add set up step that will make sure log files from previous run are not there

std::vector<std::string> ReadFromFile(const char* pPath)
{
	std::vector<std::string> content;
	std::ifstream in(pPath);
	if (in.is_open())
	{
		std::string s;
		while (std::getline(in, s))
		{
			//in >> s;
			content.push_back(s);
		}
		in.close();
	}

	return content;
}


TEST(LogTest, addLogLevel) {

    fw::debug::Logger::AddLogLevel(DEBUG_LEVEL, "c:\\temp\\log1.log", "DEBUG");
    fw::debug::Logger::SetLogDateTime(DEBUG_LEVEL, false, false);
    fw::debug::Logger::SetLogLevel(DEBUG_LEVEL, false);
    fw::debug::Logger::Log(DEBUG_LEVEL, "This is just a test");
    fw::debug::Logger::Destroy();

	std::vector<std::string> content = ReadFromFile("c:\\temp\\log1.log");
    EXPECT_EQ(1, content.size());
    ASSERT_STREQ("This is just a test", content.front().c_str());
    
}

TEST(LogTest, twoLevelsSameFile)
{
    fw::debug::Logger::AddLogLevel(LEVEL_GATE_CREATE, "c:\\temp\\TestPDBEditor.log", "GATE_CREATE");
    fw::debug::Logger::AddLogLevel(LEVEL_GATE_ADD_PIN, "c:\\temp\\TestPDBEditor.log", "LEVEL_GATE_ADD_PIN");
    fw::debug::Logger::SetLogDateTime(LEVEL_GATE_CREATE, false, false);
    fw::debug::Logger::SetLogDateTime(LEVEL_GATE_ADD_PIN, false, false);

    std::string sGateName = "gate";
    fw::debug::Logger::Log(LEVEL_GATE_CREATE, "%s", sGateName.c_str());
    for (int i = 0; i < 26; i++)
    {
        fw::debug::Logger::Log(LEVEL_GATE_ADD_PIN, "%d %s %s %s",i+1, "", "", "");
    }

	std::vector<std::string> content = ReadFromFile("c:\\temp\\TestPDBEditor.log");
    ASSERT_EQ(27, content.size());
    if (content.size() == 27)
    {
        ASSERT_STREQ("[GATE_CREATE] gate", content[0].c_str());
        ASSERT_STREQ("[LEVEL_GATE_ADD_PIN] 1   ", content[1].c_str());
        ASSERT_STREQ("[LEVEL_GATE_ADD_PIN] 26   ", content[26].c_str());
    }

}

TEST(LogTest, dontLogAnythingWhenLevelNotAdded)
{
#ifdef WIN32
    fw::debug::Logger::Destroy();
    ASSERT_NO_THROW(fw::debug::Logger::Log(LEVEL_GATE_CREATE, "%s", "GATE_CREATE"));  
#else
    FAIL(); //TODO: write implemntation of this test for platform other than windows
#endif
}

TEST(LogTest, oneLevelWithDateOtherWithout)
{
    fw::debug::Logger::AddLogLevel(LEVEL_WITH_DATETIME, "c:\\temp\\datenodate.log", "DEBUG");
    fw::debug::Logger::AddLogLevel(LEVEL_WITHOUT_DATETIME, "c:\\temp\\datenodate.log", "DEBUG");
    fw::debug::Logger::SetLogDateTime(LEVEL_WITHOUT_DATETIME, false, false);    

    fw::debug::Logger::Log(LEVEL_WITH_DATETIME, "%s", "This one has date and time");
    fw::debug::Logger::Log(LEVEL_WITHOUT_DATETIME, "%s", "This one does not have date time");

    fw::debug::Logger::Destroy();

	std::vector<std::string> content = ReadFromFile("c:\\temp\\datenodate.log");

    ASSERT_EQ(2, content.size());
    ASSERT_STRNE("[DEBUG] This one has date and time", content[0].c_str()); //checkin NON EQ as there should be date added!!!
    ASSERT_STREQ("[DEBUG] This one does not have date time", content[1].c_str());
    
}

TEST(LogTest, openLogMultipleTimesInOneSession)
{
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log", "TESTING1");
	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING, false, false);
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING, "%s", "First entry in first loop");
	fw::debug::Logger::RemoveLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log"); //this should close file!

	std::vector<std::string> content1 = ReadFromFile("c:\\temp\\looplog.log");
	ASSERT_EQ(1, content1.size());
	ASSERT_STREQ("[TESTING1] First entry in first loop", content1[0].c_str());

	//this should reopen file again
	try
	{
		fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log", "TESTING2");
		fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING, false, false);
	}
	catch (std::string e)
	{
		FAIL(); 
	}
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING, "%s", "First entry in second loop");
	fw::debug::Logger::RemoveLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log"); //this should close file!

	std::vector<std::string> content2 = ReadFromFile("c:\\temp\\looplog.log");
	ASSERT_EQ(1, content2.size());
	ASSERT_STREQ("[TESTING2] First entry in second loop", content2[0].c_str());
	
}


TEST(LogTest, closeAllTargetsForAFileAtOnce)
{
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_1, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_2, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_3, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_4, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_5, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");

	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_1, false, false);
	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_2, false, false);
	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_3, false, false);
	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_4, false, false);
	fw::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_5, false, false);

	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING_1, "%s", "First entry in first loop");
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING_2, "%s", "First entry in first loop");
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING_3, "%s", "First entry in first loop");
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING_4, "%s", "First entry in first loop");
	fw::debug::Logger::Log(LEVEL_LOOP_LOGGING_5, "%s", "First entry in first loop");

	fw::debug::Logger::RemoveAllLogLevels("c:\\temp\\looplog_many_levels.log");

	//if RemoveAllLogLevels did not remove all levels any of he lines below should throw exception!
	ASSERT_NO_THROW(fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_1, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_2, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_3, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_4, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(fw::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_5, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));

	fw::debug::Logger::Destroy();

}

void TestFormat(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fw::debug::Logger::vLog(LEVEL_LOG_AP, fmt, ap);
	va_end(ap);
}


TEST(LogTest, vLog_Test)
{

	fw::debug::Logger::AddLogLevel(LEVEL_LOG_AP, "c:\\temp\\logging_ap.log", "MANYLEVELS");
	fw::debug::Logger::SetLogDateTime(LEVEL_LOG_AP, false, false);
	TestFormat("Testing %s", "AP_LOGGING");

	std::vector<std::string> content2 = ReadFromFile("c:\\temp\\logging_ap.log");
	ASSERT_EQ(1, content2.size());
	ASSERT_STREQ("[MANYLEVELS] Testing AP_LOGGING", content2[0].c_str());

}


TEST(LogTest, logWithAppend)
{
	fw::debug::Logger::AddLogLevel(LEVEL_APPEND, "c:\\temp\\append.log", "APPEND", fw::debug::LOGGING_MODE_APPEND);
	fw::debug::Logger::SetLogDateTime(LEVEL_APPEND, false, false);
	fw::debug::Logger::Log(LEVEL_APPEND, "row 1");

	fw::debug::Logger::Destroy();

	fw::debug::Logger::AddLogLevel(LEVEL_APPEND, "c:\\temp\\append.log", "APPEND", fw::debug::LOGGING_MODE_APPEND);
	fw::debug::Logger::SetLogDateTime(LEVEL_APPEND, false, false);
	fw::debug::Logger::Log(LEVEL_APPEND, "row 2");

	std::vector<std::string> content = ReadFromFile("c:\\temp\\append.log");
	EXPECT_EQ(2, content.size());
	ASSERT_STREQ("[APPEND] row 1", content[0].c_str());
	ASSERT_STREQ("[APPEND] row 2", content[1].c_str());

}