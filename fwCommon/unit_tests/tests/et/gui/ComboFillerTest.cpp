
#include "MyCombo.h"


TEST(ComboFillerTest, GUIItemShouldReturnTitle) {

    MyItem m(1, "title");
    EXPECT_EQ(1, (int)m.GetData());
    ASSERT_STREQ("title", m.GetTitle().c_str());
}


TEST(ComboFillerTest, ComboFillerTest) {

    MyCombo combo;
    et::gui::ComboFiller<MyCombo> cf(&combo);
    std::vector<MyItem*> collection;
    collection.push_back(new MyItem(1, "one"));
    collection.push_back(new MyItem(2, "two"));

    std::for_each(collection.begin(), collection.end(), cf);

    EXPECT_EQ(2, combo.GetCount());

    std::vector<MyItem*>::iterator it;
    for (it = collection.begin(); it < collection.end(); it++)
        delete *it;
}