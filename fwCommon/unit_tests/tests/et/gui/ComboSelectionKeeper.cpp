#include "MyCombo.h"

TEST(ComboSelectionKeeperTest, SelKeeperShouldRememberCurrentSelection) {

    MyCombo combo;
    combo.AddString("one");
    combo.AddString("two");
    combo.AddString("three");
    combo.SetCurSel(1); //two
    {
        fw::gui::ComboSelectionKeeper<MyCombo> csk(&combo);
        combo.ResetContent();
        combo.AddString("three");
        combo.AddString("one");
        combo.AddString("two");
    }

    CString s;
    combo.GetLBText(combo.GetCurSel(), s);
    EXPECT_EQ("two", s);
    
}


TEST(ComboSelectionKeeperTest, SelKeeperShouldRestoreSelectionBasedOnString) {

    MyCombo combo;
    combo.AddString("8");
    combo.AddString("7");
    combo.AddString("4");
    combo.AddString("12");
    combo.AddString("1");
    combo.AddString("0");
    {
        CString sStringToSelect;
        sStringToSelect = "12";
        fw::gui::ComboSelectionKeeper<MyCombo> csk(&combo, sStringToSelect);
    }

    CString s;
    combo.GetLBText(combo.GetCurSel(), s);
    EXPECT_EQ("12", s);
}

TEST(ComboSelectionKeeperTest, SelKeeperShouldRestoreSelectionBasedOnData) {

    MyCombo combo;
    int iIndex = combo.AddString("this");
    combo.SetItemData(iIndex, (DWORD_PTR)1);

    iIndex = combo.AddString("is");
    combo.SetItemData(iIndex, (DWORD_PTR)102);

    iIndex = combo.AddString("test");
    combo.SetItemData(iIndex, (DWORD_PTR)3);

    {
        DWORD_PTR pData = 102;
        fw::gui::ComboSelectionKeeper<MyCombo> csk(&combo, pData);
    }

    CString s;
    combo.GetLBText(combo.GetCurSel(), s);
    EXPECT_EQ("is", s);

}