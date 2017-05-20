#ifndef FWCOMMON_H_INCLUDED
#define FWCOMMON_H_INCLUDED

typedef unsigned int Identifier;

#define InvalidID -1

/** @brief Invalid coordinace */
#define InvalidCoord -1


/** @brief Empty string */
#define SIEmptyString _T("")

/**
@brief Week days enumeration. The order is correlated with
order given for example by CTime::GetDayOfWeek.
*/
enum WeekDayEnum {
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
};

//enum with all known image types
enum ImageTypeEnum
{
	IMG_TYPE_UNKNOWN,
	IMG_TYPE_BMP,
	IMG_TYPE_PNG,
	IMG_TYPE_GIF,
	IMG_TYPE_JPG
};


//how value of the field should be align
//in the column
enum AlignEnum
{
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};

typedef ImageTypeEnum ImageType;


/**
@brief
Full names of week days (monday, tuesday, etc...).
This array is correlated with WeekDayEnum!
*/
extern CString WeekDayNames[];

/**
@brief A helper macro to simplyify
code related to flag checking
*/
#define HAS_STYLE(style, flag) ((style & flag) != 0)


/**
@brief Calculates the procentage progress
based on the total and current values
*/
#define PROC_PROGRESS(current, total)  ((current*100)/total)

/**
This is wrapper to fw::core::TextConv::Unicode2UTF8
(just to simplyfiy the code)
*/
//extern std::string DLLEXPORT Unicode2UTF8(const CString& pUnicodeStr);


/**
This is wrapper to fw::core::TextConv::UTF82Unicode
(just to simplify the code)
*/
//extern CString DLLEXPORT fw::core::TextConv::UTF82Unicode(const char* pUTF8Str);

//----------------------------------------------------------------UNICODE-BEGIN
//Defines in this section are made to simplyfy code
//between UNICODE and non-UNIOCDE version
//these defines are made for STL and boost

#ifdef UNICODE
#define std_string std::wstring

#else //non-UNICODCE
#define std_string std::string
#endif



#endif //FWCOMMON_H_INCLUDED
