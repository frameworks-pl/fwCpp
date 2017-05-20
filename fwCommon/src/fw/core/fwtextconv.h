#ifndef SITEXTCONV_H_INCLUDED
#define SITEXTCONV_H_INCLUDED


namespace fw {

	namespace core 
	{

		/**
		  This class implements handy conversion functions
		  like Unicode->UTF8, UTF8->Unicode, etc.
		*/
		class TextConv 
		{

			public:
				static std::string Unicode2UTF8(const CString& pUnicodeStr);
				static CString UTF82Unicode(const char* pUTF8Str);

				//removes breaks from the specified string
				static void StripBreaks(CString& pString);

				//removes all quotation marks from the string and
				//then adds one at the beginning and one at the end
				static void Quote(CString& pString);


		}; //class

	} //namespace core


} //namespace





#endif //SITEXTCONV_H_INCLUDED