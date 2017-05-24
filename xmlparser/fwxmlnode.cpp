#include "fwxmlnode.h"
#include <string>
#include <src/fw/core/fwnumbers.h>


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

XMLAttribute* FWXMLNode::addAttribute(XMLNode& pXMLNode, const CString& pAttrName, const CString pAttrValue)
{
  return pXMLNode.addAttribute((LPCTSTR)pAttrName, (LPCTSTR)pAttrValue);

}

XMLAttribute* FWXMLNode::addAttribute(XMLNode& pXMLNode, const CString& pAttrName, int pAttrValue)
{
  CString s;
  s.Format(_T("%d"), pAttrValue);
  return pXMLNode.addAttribute((LPCTSTR)pAttrName, (LPCTSTR)s);

}


bool FWXMLNode::getAttributeInt(const XMLNode& pXMLNode, const CString& pAttrName, int& pAttrValue)
{
  const wchar_t* pAttrVal = pXMLNode.getAttribute((LPCTSTR)pAttrName);
  if (NULL != pAttrVal)
  {
    CString s(pAttrVal);
    if (fw::core::Numbers::isInt(s, true))
    {
      pAttrValue = fw::core::Numbers::toInt(s);
      return true;
    }
  }

  return false;

}

bool FWXMLNode::getAttributeBool(const XMLNode& pXMLNode, const CString& pAttrName, bool& pAttrValue)
{
  int iValue = 0;
  if (getAttributeInt(pXMLNode, pAttrName, iValue))
  {
    pAttrValue = (iValue != 0);
    return true;
  }

  return false;

}


void FWXMLNode::setText(XMLNode& pXMLNode, const CString& pText)
{
  pXMLNode.updateText((LPCTSTR)pText);

}

void FWXMLNode::setText(XMLNode& pXMLNode, const TCHAR& pChar)
{
  pXMLNode.updateText(&pChar);

}


void FWXMLNode::setText(XMLNode& pXMLNode, int pIntValue)
{
  CString s;
  s.Format(_T("%d"), pIntValue);
  pXMLNode.updateText((LPCTSTR)s);

}


void FWXMLNode::setText(XMLNode& pXMLNode, unsigned int pUintValue)
{
  CString s;
  s.Format(_T("%u"), pUintValue);
  pXMLNode.updateText((LPCTSTR)s);


}



bool FWXMLNode::getTextInt(const XMLNode& pXMLNode, int& pIntValue)
{
  CString s((LPCTSTR)pXMLNode.getText());
  if (fw::core::Numbers::isInt(s, true))
  {
    pIntValue = fw::core::Numbers::toInt(s);
    return true;
  }

  return false;

}


CString FWXMLNode::nodeToString(const XMLNode& pXMLNode)
{
  wchar_t* pXMLString = pXMLNode.createXMLString();
  CString sResult(pXMLString);
  free(pXMLString);

  return sResult;


}


bool FWXMLNode::getAttributeString(const XMLNode& pXMLNode, const CString& pAttrName, CString& pAttrValue)
{
  const wchar_t* pAttr = pXMLNode.getAttribute((LPCTSTR)pAttrName);
  if (NULL == pAttr)
    return false;

  pAttrValue = (LPCTSTR)pAttr;
  return true;


}