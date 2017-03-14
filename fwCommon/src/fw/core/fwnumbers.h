#ifndef SINUMBERS_H_INCLUDED
#define SINUMBERS_H_INCLUDED


namespace fw 
{
	namespace core
	{

		class  Numbers
		{
			public:
				Numbers();
				static bool isInt(const CString& sValue, bool bSigned = false);
				static int toInt(const CString& pSource);
				/**
				@brief Returns true if sValue is float number
				@param pDecPoint - a character representing decimal point
				*/
				static bool isFloat(const CString& sValue, const TCHAR& pDecPoint, bool bSigned = false);
				static double toFloat(const CString& pSource, const TCHAR& pDecPoint = _T('.'));

				//converts double to integer with rounding
				static int doubleToIntWithRounding(double pDouble);

			protected:
				static inline bool isDigit(const TCHAR& pChar);


		}; //class

	} //namespace core

}; //namespace

#endif //SINUMBERS_H_INCLUDED