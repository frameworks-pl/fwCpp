#include "fwCommon_pch.h"
#include "fwnumbers.h"
#include "fwexception.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace fw 
{

	namespace core
	{

		Numbers::Numbers() 
		{

		}; //Numbers

		bool Numbers::isDigit(const TCHAR& pChar) 
		{
			CString sNumbers = _T("0123456789");
			for (int i = 0; i < sNumbers.GetLength(); i++)
				if (sNumbers.GetAt(i) == pChar)
					return true;

			return false;

		}; //isDigit


		bool Numbers::isInt(const CString& sValue, bool bSigned) 
		{

			if (sValue.IsEmpty())
				return false;

			TCHAR plus = _T('+');
			TCHAR minus = _T('-');

			int count = sValue.GetLength();
			for (int i = 0; i < count; i++) {

				TCHAR c = sValue.GetAt(i);

				//if the number is signed
				//the first character can only be plus, minus or a number
				if (bSigned && !i && c != plus && c != minus && !isDigit(c))
					return false;

				if ((!bSigned || i) && !isDigit(sValue.GetAt(i)))
					return false;
			};

			return true;


		}; //isInt

		int Numbers::toInt(const CString& pSource) 
		{

			TCHAR* pStop = NULL;
			CString sSource = pSource;
			int iResult = _tcstol(sSource.GetBuffer(0), &pStop, 10);

			if (*pStop) {
				throw Exception(_T("Numbers::toInt failed!"));
			}


			return iResult;
		}; //toInt

		double Numbers::toFloat(const CString& pSource, const TCHAR& pDecPoint)
		{

			TCHAR* pStop = NULL;
			CString sSource = pSource;

			//_tcstod expects a "dot" as a decimal point, so we have to 
			//convert the value first
			if (_T('.') != pDecPoint)
				sSource.Replace(pDecPoint, _T('.'));

			double dResult = _tcstod(sSource.GetBuffer(0), &pStop);

			if (*pStop)
				throw Exception(_T("Numbers::toFloat failed!"));

			return dResult;

		}; //toFloat

		bool Numbers::isFloat(const CString& sValue, const TCHAR& pDecPoint, bool bSigned) 
		{

			if (sValue.IsEmpty())
				return false;

			TCHAR plus = _T('+');
			TCHAR minus = _T('-');

			int count = sValue.GetLength();
			for (int i = 0; i < count; i++) {

				TCHAR c = sValue.GetAt(i);

				if (bSigned && !i && c != plus && c != minus && !isDigit(c))
					return false;

				if (i && (!isDigit(c) && c != pDecPoint))
					return false;
			};

			return true;

		}; //isFloat


		int Numbers::doubleToIntWithRounding(double pDouble)
		{

			int iValue = (int)pDouble;

			double dValue = (double)iValue;
			double dFraction = pDouble - dValue;

			if (dFraction > std::numeric_limits<double>::epsilon())
			{
				if (dFraction > 0.5)
					iValue += 1;
			}

			return iValue;

		}

	} //namespace core

}; //namespace