#ifndef FWREGEXH_INCLUDED
#define FWREGEXH_INCLUDED


#include <utility>
#include <boost/regex/mfc.hpp> 


//#include <boost/regex.hpp>

namespace fw 
{
	namespace core {


		//simple structure to keep chunks of a string
		//with information wether they are links or not
		struct ChunkInfo
		{
			ChunkInfo(const CString& pChunk, bool pIsURL)
			{
				m_sChunk = pChunk;
				m_bIsURL = pIsURL;
			}

			CString m_sChunk;
			bool m_bIsURL;
		};

		typedef std::list<fw::core::ChunkInfo> ChunkList;


		/**
		  Class that holds helper functions that use
		  regular expressions to perform some kind of task!
		*/
		class RegEx {

			public:

				//Enum specifing what kind of links should be extraced
				//by (for example) parseURLS
				enum LinkTypeEnum
				{
					LINK_WWW = 0x1,
					LINK_EMAIL = 0x2
				};


				/**
				  Searches for the specified regular expression in the pString.
				  If the string is found its location (start,end) is returned. Otherwise (-1, -1)
				  is returned.
				*/
				static std::pair<int, int> getMatchLocaltion(const CString& pString, const CString& pRegEx, bool pCaseSensitive = false);

				/**
				  Searches for the specified regular expresion in the pString.
				  The result is the first substring that matched the regular expression
				*/
				static CString getMatchString(const CString& pString, const CString& pRegEx, bool pCaseSensitive = false);

				/**
				  The method returns true if the specified string matches (exactly) the specified
				  regular expression
				*/
				static bool match(const CString& pString, const CString& pRegEx, bool pCaseSensitive = false);

				/*
				  Takes the specified text and converts it into a list of chunks, where
				  each chank is either a simple text or an URL (www, e-mail)
				  bIncludeEmails specifies whether e-mails should be included or not
				*/
				static bool parseURLS(const CString& pText, std::list<ChunkInfo>& pChunks, int iLinkTypes);

				/**
				  The mask that represents a valid e-mail address
				*/
				static const CString EMAIL_REGEX;

				/**
				  The mask that represents a valid link
				*/
				static const CString URL_REGEX;

			protected:

				/**
				  @brief A helper function that creates std::string
				  containing the specified match from the boost::cmatch
				  structure
				*/
				static std_string getMatch(int pMatchNo, boost::tmatch& pMatches);

		}; //class

	} //namespace core

} //fw



#endif //FWREGEXH_INCLUDED