#include "sixmlnode.h"
#include <string>
#include <src/fw/core/fwnumbers.h>


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

XMLAttribute* SIXMLNode::addAttribute(XMLNode& pXMLNode, const CString& pAttrName, const CString pAttrValue)
{
  return pXMLNode.addAttribute((LPCTSTR)pAttrName, (LPCTSTR)pAttrValue);

}

XMLAttribute* SIXMLNode::addAttribute(XMLNode& pXMLNode, const CString& pAttrName, int pAttrValue)
{
  CString s;
  s.Format(_T("%d"), pAttrValue);
  return pXMLNode.addAttribute((LPCTSTR)pAttrName, (LPCTSTR)s);

}


bool SIXMLNode::getAttributeInt(const XMLNode& pXMLNode, const CString& pAttrName, int& pAttrValue)
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

bool SIXMLNode::getAttributeBool(const XMLNode& pXMLNode, const CString& pAttrName, bool& pAttrValue)
{
  int iValue = 0;
  if (getAttributeInt(pXMLNode, pAttrName, iValue))
  {
    pAttrValue = (iValue != 0);
    return true;
  }

  return false;

}


void SIXMLNode::setText(XMLNode& pXMLNode, const CString& pText)
{
  pXMLNode.updateText((LPCTSTR)pText);

}

void SIXMLNode::setText(XMLNode& pXMLNode, const TCHAR& pChar)
{
  pXMLNode.updateText(&pChar);

}


void SIXMLNode::setText(XMLNode& pXMLNode, int pIntValue)
{
  CString s;
  s.Format(_T("%d"), pIntValue);
  pXMLNode.updateText((LPCTSTR)s);

}


void SIXMLNode::setText(XMLNode& pXMLNode, unsigned int pUintValue)
{
  CString s;
  s.Format(_T("%u"), pUintValue);
  pXMLNode.updateText((LPCTSTR)s);


}



bool SIXMLNode::getTextInt(const XMLNode& pXMLNode, int& pIntValue)
{
  CString s((LPCTSTR)pXMLNode.getText());
  if (fw::core::Numbers::isInt(s, true))
  {
    pIntValue = fw::core::Numbers::toInt(s);
    return true;
  }

  return false;

}


CString SIXMLNode::nodeToString(const XMLNode& pXMLNode)
{
  wchar_t* pXMLString = pXMLNode.createXMLString();
  CString sResult(pXMLString);
  free(pXMLString);

  return sResult;


}


bool SIXMLNode::getAttributeString(const XMLNode& pXMLNode, const CString& pAttrName, CString& pAttrValue)
{
  const wchar_t* pAttr = pXMLNode.getAttribute((LPCTSTR)pAttrName);
  if (NULL == pAttr)
    return false;

  pAttrValue = (LPCTSTR)pAttr;
  return true;


}