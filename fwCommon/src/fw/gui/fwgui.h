#ifndef FWGUI_H_INCLUDED
#define FWGUI_H_INCLUDED


namespace fw
{
    namespace gui
    {
        //--------------------------------------------------------------------------------
        //Base class for items in collections that are to be filled by tools like ComboFiller
        class Item
        {
            public:
                virtual ~Item() { }
                virtual DWORD_PTR GetData() = 0;
                virtual std::string GetTitle() = 0;
        };
        //--------------------------------------------------------------------------------
        

        //--------------------------------------------------------------------------------
        template <class T>
        class ComboFiller
        {
            public:
                ComboFiller(T* pComboBox) : m_pComboBox(pComboBox) { }
                void operator()(Item* pItem)
                {
                    AFX_MANAGE_STATE(AfxGetStaticModuleState());
                    int iIndex = m_pComboBox->AddString(pItem->GetTitle().c_str());
                    m_pComboBox->SetItemData(iIndex, (DWORD_PTR)pItem->GetData());

                }
            protected:
                T* m_pComboBox;
        };
        //--------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------
        template <class T>
        class ComboSelectionKeeper
        {
            public:
                ComboSelectionKeeper(T* pComboBox) : m_pComboBox(pComboBox), m_pData(NULL), m_iIndex(-1)
                {

                    if (m_pComboBox && m_pComboBox->m_hWnd)
                    {
                        int iCurSel = m_pComboBox->GetCurSel();
                        if (iCurSel != -1)
                            m_pComboBox->GetLBText(iCurSel, m_sString);
                    }
                }

                ComboSelectionKeeper(T* pComboBox, DWORD_PTR pData)
                    : m_pComboBox(pComboBox), m_pData(pData), m_iIndex(-1)
                {
                }

                ComboSelectionKeeper(T* pComboBox, const CString& pString)
                    : m_pComboBox(pComboBox), m_pData(NULL), m_sString(pString), m_iIndex(-1) {
                }

                ComboSelectionKeeper(T* pComboBox, int iIndex)
                    : m_pComboBox(pComboBox), m_pData(NULL) {
                }

                virtual ~ComboSelectionKeeper()
                {
                    if (m_pComboBox && m_pComboBox->m_hWnd)
                    {
                        //if selection is kept as index, we use it to extract the string
                        //under that index and then will use string matching to make the selection
                        if (m_iIndex != -1 && m_pComboBox->GetCount() < m_iIndex)
                            m_pComboBox->GetLBText(m_iIndex, m_sString);

                        int iCount = m_pComboBox->GetCount();
                        for (int i = 0; i < iCount; i++)
                        {
                            CString sCurSel;
                            if (m_pData != NULL)
                            {
                                DWORD_PTR pData = m_pComboBox->GetItemData(i);
                                if (pData == m_pData)
                                {
                                    m_pComboBox->SetCurSel(i);
                                    return;
                                }
                            }
                            else if (m_sString.IsEmpty() == false) {
                                CString sCurSel;
                                m_pComboBox->GetLBText(i, sCurSel);
                                if (sCurSel.Compare(m_sString) == 0)
                                {
                                    m_pComboBox->SetCurSel(i);
                                    return;
                                }
                            }
                        }

                        //did not find string to select, select first thing in combo
                        m_pComboBox->SetCurSel(0);
                    }
                }
            protected:
                T* m_pComboBox;
                DWORD_PTR m_pData;
                CString m_sString; //one of strings in combo
                int m_iIndex; //index of the value in combo
        };
        //--------------------------------------------------------------------------------

    } //namespace gui
} //namespace fw

#endif //FWGUI_H_INCLUDED