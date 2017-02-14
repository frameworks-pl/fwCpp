#define _USING_V110_SDK71_

#include "gtest.h"
#include "ettest.h"
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <src/et/debug/etlog.h>

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

    et::debug::Logger::AddLogLevel(DEBUG_LEVEL, "c:\\temp\\log1.log", "DEBUG");
    et::debug::Logger::SetLogDateTime(DEBUG_LEVEL, false, false);
    et::debug::Logger::SetLogLevel(DEBUG_LEVEL, false);
    et::debug::Logger::Log(DEBUG_LEVEL, "This is just a test");
    et::debug::Logger::Destroy();

	std::vector<std::string> content = ReadFromFile("c:\\temp\\log1.log");
    EXPECT_EQ(1, content.size());
    ASSERT_STREQ("This is just a test", content.front().c_str());
    
}

TEST(LogTest, twoLevelsSameFile)
{
    et::debug::Logger::AddLogLevel(LEVEL_GATE_CREATE, "c:\\temp\\TestPDBEditor.log", "GATE_CREATE");
    et::debug::Logger::AddLogLevel(LEVEL_GATE_ADD_PIN, "c:\\temp\\TestPDBEditor.log", "LEVEL_GATE_ADD_PIN");
    et::debug::Logger::SetLogDateTime(LEVEL_GATE_CREATE, false, false);
    et::debug::Logger::SetLogDateTime(LEVEL_GATE_ADD_PIN, false, false);

    CString sGateName = "gate";
    et::debug::Logger::Log(LEVEL_GATE_CREATE, "%s", sGateName);
    for (int i = 0; i < 26; i++)
    {
        et::debug::Logger::Log(LEVEL_GATE_ADD_PIN, "%d %s %s %s",i+1, "", "", "");
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
    et::debug::Logger::Destroy();
    ASSERT_NO_THROW(et::debug::Logger::Log(LEVEL_GATE_CREATE, "%s", "GATE_CREATE"));  
#else
    FAIL(); //TODO: write implemntation of this test for platform other than windows
#endif
}

TEST(LogTest, oneLevelWithDateOtherWithout)
{
    et::debug::Logger::AddLogLevel(LEVEL_WITH_DATETIME, "c:\\temp\\datenodate.log", "DEBUG");
    et::debug::Logger::AddLogLevel(LEVEL_WITHOUT_DATETIME, "c:\\temp\\datenodate.log", "DEBUG");
    et::debug::Logger::SetLogDateTime(LEVEL_WITHOUT_DATETIME, false, false);    

    et::debug::Logger::Log(LEVEL_WITH_DATETIME, "%s", "This one has date and time");
    et::debug::Logger::Log(LEVEL_WITHOUT_DATETIME, "%s", "This one does not have date time");

    et::debug::Logger::Destroy();

	std::vector<std::string> content = ReadFromFile("c:\\temp\\datenodate.log");

    ASSERT_EQ(2, content.size());
    ASSERT_STRNE("[DEBUG] This one has date and time", content[0].c_str()); //checkin NON EQ as there should be date added!!!
    ASSERT_STREQ("[DEBUG] This one does not have date time", content[1].c_str());
    
}

TEST(LogTest, openLogMultipleTimesInOneSession)
{
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log", "TESTING1");
	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING, false, false);
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING, "%s", "First entry in first loop");
	et::debug::Logger::RemoveLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log"); //this should close file!

	std::vector<std::string> content1 = ReadFromFile("c:\\temp\\looplog.log");
	ASSERT_EQ(1, content1.size());
	ASSERT_STREQ("[TESTING1] First entry in first loop", content1[0].c_str());

	//this should reopen file again
	try
	{
		et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log", "TESTING2");
		et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING, false, false);
	}
	catch (std::string e)
	{
		FAIL(); 
	}
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING, "%s", "First entry in second loop");
	et::debug::Logger::RemoveLogLevel(LEVEL_LOOP_LOGGING, "c:\\temp\\looplog.log"); //this should close file!

	std::vector<std::string> content2 = ReadFromFile("c:\\temp\\looplog.log");
	ASSERT_EQ(1, content2.size());
	ASSERT_STREQ("[TESTING2] First entry in second loop", content2[0].c_str());
	
}


TEST(LogTest, closeAllTargetsForAFileAtOnce)
{
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_1, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_2, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_3, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_4, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");
	et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_5, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS");

	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_1, false, false);
	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_2, false, false);
	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_3, false, false);
	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_4, false, false);
	et::debug::Logger::SetLogDateTime(LEVEL_LOOP_LOGGING_5, false, false);

	et::debug::Logger::Log(LEVEL_LOOP_LOGGING_1, "%s", "First entry in first loop");
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING_2, "%s", "First entry in first loop");
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING_3, "%s", "First entry in first loop");
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING_4, "%s", "First entry in first loop");
	et::debug::Logger::Log(LEVEL_LOOP_LOGGING_5, "%s", "First entry in first loop");

	et::debug::Logger::RemoveAllLogLevels("c:\\temp\\looplog_many_levels.log");

	//if RemoveAllLogLevels did not remove all levels any of he lines below should throw exception!
	ASSERT_NO_THROW(et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_1, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_2, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_3, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_4, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));
	ASSERT_NO_THROW(et::debug::Logger::AddLogLevel(LEVEL_LOOP_LOGGING_5, "c:\\temp\\looplog_many_levels.log", "MANYLEVELS"));

	et::debug::Logger::Destroy();

}

void TestFormat(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	et::debug::Logger::vLog(LEVEL_LOG_AP, fmt, ap);
	va_end(ap);
}


TEST(LogTest, vLog_Test)
{

	et::debug::Logger::AddLogLevel(LEVEL_LOG_AP, "c:\\temp\\logging_ap.log", "MANYLEVELS");
	et::debug::Logger::SetLogDateTime(LEVEL_LOG_AP, false, false);
	TestFormat("Testing %s", "AP_LOGGING");

	std::vector<std::string> content2 = ReadFromFile("c:\\temp\\logging_ap.log");
	ASSERT_EQ(1, content2.size());
	ASSERT_STREQ("[MANYLEVELS] Testing AP_LOGGING", content2[0].c_str());

}