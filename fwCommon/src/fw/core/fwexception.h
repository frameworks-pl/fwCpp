#ifndef SIEXCEPTION_H_INCLUDED
#define SIEXCEPTION_H_INCLUDED

#include <exception>

namespace fw 
{

	namespace core
	{

		class Exception : public std::exception {

		public:
			Exception(const CString& sMsg);
			//SIException(const SIException& sie);
			const CString& getMessage() const { return mMessage; }
		protected:
			CString mMessage;

		};
	} //namespace core

}; //namespace


#endif //SIEXCEPTION_H_INCLUDED