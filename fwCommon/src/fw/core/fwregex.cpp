#include "fwCommon_pch.h"
#include "fwregex.h"
#include "fwexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace fw {

	namespace core {

		const CString RegEx::EMAIL_REGEX = _T("([\\w\\.-]+@[\\w-]+[^\\s]+)");
		const CString RegEx::URL_REGEX = _T("((http://[\\w-]+[^\\s]*)|(https://[\\w-]+[^\\s]*)|(www\\.[\\w-]+[^\\s]*))");

		std::pair<int, int>
			RegEx::getMatchLocaltion(const CString& pString, const CString& pRegEx, bool pCaseSensitive) {



			//At least one pair of brackets must be found
			//in the regular expression string
			//that's because the callers specifies what exactly should be 
			//pull out of the string!!
			if (pRegEx.Find(_T("(")) == -1 || pRegEx.Find(_T(")")) == -1)
				throw fw::core::Exception(_T("The regular expression string does not include any brackets, don't know what you're looking for!!"));


			//structre that receives all parts matched by the bracets

			std::pair<int, int> result(-1, -1);

			int flags = boost::tregex::perl;
			if (false == pCaseSensitive)
				flags |= boost::tregex::icase;
			boost::tmatch matches;
			boost::tregex re((LPCTSTR)pRegEx, flags);


			//if (regex_match((LPCTSTR)pString, matches, re)) {
			if (regex_search((LPCTSTR)pString, matches, re))
			{

				//we are assuming here that user wants the postion
				//matched by the first pair of bractes
				//(other brackets are ignored)
				CString match_str(getMatch(1, matches).c_str());

				//Looking for the postion of the matched string
				//in the original string
				result.first = pString.Find(match_str, 0);
				result.second = result.first + match_str.GetLength();
			}


			return result;

		}; //getMatchLocation


		CString
			RegEx::getMatchString(const CString& pString, const CString& pRegEx, bool pCaseSensitive) {

			CString result;
			std::pair<int, int> location = getMatchLocaltion(pString, pRegEx, pCaseSensitive);

			if (location.first != -1 && location.second != -1)
				result = pString.Mid(location.first, (location.second - location.first));

			return result;

		}; //getMatchString



		bool
			RegEx::match(const CString& pString, const CString& pRegEx, bool pCaseSensitive)
		{


			int flags = boost::tregex::perl;
			if (false == pCaseSensitive)
				flags |= boost::tregex::icase;

			boost::tmatch matches;
			boost::tregex re((LPCTSTR)pRegEx, flags);

			return (regex_match((LPCTSTR)pString, matches, re));

		}; //match


		std_string
			RegEx::getMatch(int pMatchNo, boost::tmatch& pMatches) {

			std_string the_match;
			the_match.assign(pMatches[pMatchNo].first, pMatches[pMatchNo].second);
			return the_match;

		};


		bool RegEx::parseURLS(const CString& pText, std::list<ChunkInfo>& pChunks, int iLinkTypes)
		{
			bool bAtLeastOneURLFound = false;

			pChunks.clear();
			if (pText.GetLength() > 0)
			{
				CString sentence = pText;

				CString sLinksRE;
				if (HAS_STYLE(iLinkTypes, LINK_EMAIL))
					sLinksRE = fw::core::RegEx::EMAIL_REGEX;

				if (HAS_STYLE(iLinkTypes, LINK_WWW))
				{
					if (sLinksRE.IsEmpty())
						sLinksRE = URL_REGEX;
					else
					{
						sLinksRE.Format(_T("(%s|%s)"), sLinksRE, URL_REGEX);
						//sLinksRE += _T("|") + ;
					}
				}


				std::pair<int, int> one_match;
				do
				{
					one_match = fw::core::RegEx::getMatchLocaltion(sentence, sLinksRE);
					if (one_match != std::pair<int, int>(-1, -1))
					{

						bAtLeastOneURLFound = true;

						//extract part before the url
						if (one_match.first > 0)
						{
							CString not_url_string = sentence.Mid(0, one_match.first);
							sentence = sentence.Right(sentence.GetLength() - one_match.first);
							pChunks.push_back(ChunkInfo(not_url_string, false));
							int offset = one_match.first;
							one_match.first -= offset;
							one_match.second -= offset;

						}

						CString result = sentence.Mid(one_match.first, (one_match.second - one_match.first));
						pChunks.push_back(ChunkInfo(result, true));

						//cut of result from the string
						sentence = sentence.Right(sentence.GetLength() - one_match.second);

					}
					else if (false == sentence.IsEmpty())
					{
						pChunks.push_back(ChunkInfo(sentence, false));
					}
				} while (one_match != std::pair<int, int>(-1, -1));
			}

			//if nothing was matched, return full sentence as the only element
			if (pChunks.size() == 0 && (false == pText.IsEmpty()))
				pChunks.push_back(ChunkInfo(pText, false));

			return bAtLeastOneURLFound;

		}

	} //namespace core

}; //namespace fw