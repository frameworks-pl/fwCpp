#ifndef SIXMLNODE_H_INCLUDED
#define SIXMLNODE_H_INCLUDED

#include "XMLParser.h"

#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER _WIN32_WINNT_WIN7 		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#include <afxwin.h>         // MFC core and standard components



//This is my simple wrapper that extends existing wrapper by providing tools
class SIXMLNode
{

  public:

    static XMLAttribute* addAttribute(XMLNode& pXMLNode, const CString& pAttrName, const CString pAttrValue);
    static XMLAttribute* addAttribute(XMLNode& pXMLNode, const CString& pAttrName, int pAttrValue);

    static bool getAttributeInt(const XMLNode& pXMLNode, const CString& pAttrName, int& pAttrValue);
    static bool getAttributeBool(const XMLNode& pXMLNode, const CString& pAttrName, bool& pAttrValue);
    static bool getAttributeString(const XMLNode& pXMLNode, const CString& pAttrName, CString& pAttrValue);

    static void setText(XMLNode& pXMLNode, const CString& pText);
    static void setText(XMLNode& pXMLNode, const TCHAR& pChar);
    static void setText(XMLNode& pXMLNode, int pIntValue);
    static void setText(XMLNode& pXMLNode, unsigned int pUintValue);

    static bool getTextInt(const XMLNode& pXMLNode, int& pIntValue);

    static CString nodeToString(const XMLNode& pXMLNode);


}; //class


#endif //SIXMLNODE_H_INCLUDED