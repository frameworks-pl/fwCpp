#ifndef MYCOMBO_H_INCLUDED
#define MYCOMBO_H_INCLUDED

#include <algorithm>
#include <map>
#include <basetsd.h>
#include "gtest.h"
#include "ettest.h"
#include <src/et/core/etcore.h>
#include <src/et/gui/etgui.h>

struct ComboItem
{
    std::string m_Title;
    DWORD_PTR m_pData;
};

class MyCombo
{
    public:
        MyCombo() : m_hWnd((void*)1), m_iCurSel(-1) { } //by default combo is initilzed

        int GetCount() {
            return (int)m_collection.size();
        }

        int AddString(const char* item) {
            std::string s(item);
            ComboItem ci;
            ci.m_Title = s;
            ci.m_pData = NULL;
            int iNextRow = m_collection.size();
            m_collection.insert(std::map<int, ComboItem>::value_type(iNextRow, ci));
            return (int)m_collection.size() - 1;
        }

        void SetItemData(int iIndex, DWORD_PTR pItemData) {
            std::map<int, ComboItem>::iterator it = m_collection.find(iIndex);
            if (it != m_collection.end())
                it->second.m_pData = pItemData;
        }

        DWORD_PTR GetItemData(int iIndex)
        {
            std::map<int, ComboItem>::iterator it = m_collection.find(iIndex);
            if (it != m_collection.end())
                return it->second.m_pData;
            return NULL;
        }

        int GetCurSel() {
            return m_iCurSel;
        }

        void SetCurSel(int i) {
            m_iCurSel = i;
        }

        void GetLBText(int iIndex, CString& pResult)
        {
            pResult = "";
            std::map<int, ComboItem>::iterator it = m_collection.find(iIndex);
            if (it != m_collection.end())
            {
                pResult = it->second.m_Title.c_str();
            }
        }

        void ResetContent()
        {
            m_collection.clear();
            m_iCurSel = -1;
        }

        void* m_hWnd;
    protected:
        std::map<int, ComboItem> m_collection;
        int m_iCurSel;
};

class MyItem : public et::gui::Item
{
    public:
        MyItem(int id, const std::string& title) : m_id(id), m_title(title) { }
        DWORD_PTR GetData() { return m_id; }
        std::string GetTitle() { return m_title; }
    protected:
        int m_id;
        std::string m_title;
};



#endif //MYCOMBO_H_INCLUDED