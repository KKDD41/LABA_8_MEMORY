#include <vector>
#include "bidirectional_list.h"
#include <gtest/gtest.h>

TEST(Test_0, InitListTesting) {
  containers::BiDirectionalList<int> list({1, 2, 3, 4, 5});
  std::vector<int> exp_list{1, 2, 3, 4, 5};
  EXPECT_TRUE(exp_list == list.ToVector());
}

TEST(Test_1, PushPopTesting) {
  {
    containers::BiDirectionalList<int> list;
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.IsEmpty());
    list.PushBack(11);
    list.PushBack(22);
    list.PushBack(33);
    list.PushBack(44);
    EXPECT_EQ(list.Size(), 4);
    EXPECT_FALSE(list.IsEmpty());
    std::vector<int> v_list = list.ToVector();
    std::vector<int> exp_list{11, 22, 33, 44};
    EXPECT_TRUE(v_list == exp_list);
    list.PopBack();
    exp_list = {11, 22, 33};
    EXPECT_TRUE(list.ToVector() == exp_list);
    list.PopBack();
    exp_list = {11, 22};
    EXPECT_TRUE(list.ToVector() == exp_list);
    list.PopBack();
    list.PopBack();
    EXPECT_TRUE(list.IsEmpty());
  }
  {
    containers::BiDirectionalList<int> list;
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.IsEmpty());
    list.PushFront(33);
    list.PushBack(44);
    list.PushFront(22);
    list.PushFront(11);
    EXPECT_EQ(list.Size(), 4);
    EXPECT_FALSE(list.IsEmpty());
    std::vector<int> v_list = list.ToVector();
    std::vector<int> exp_list{11, 22, 33, 44};
    EXPECT_TRUE(v_list == exp_list);
    list.PopFront();
    list.PopFront();
    exp_list = {33, 44};
    EXPECT_TRUE(list.ToVector() == exp_list);
  }
}

TEST(Test_2, InsertChecking) {
  {
    containers::BiDirectionalList<int> list;
    list.PushBack(11);
    list.PushBack(22);
    list.PushBack(33);
    list.PushBack(55);
    list.InsertBefore(list.Front(), 0);
    std::vector<int> exp_list{0, 11, 22, 33, 55};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.InsertBefore(list.Front(), -11);
    exp_list = {-11, 0, 11, 22, 33, 55};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.InsertAfter(list.Back(), 66);
    list.InsertAfter(list.Back(), 88);
    exp_list = {-11, 0, 11, 22, 33, 55, 66, 88};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.InsertBefore(list[5], 44);
    list.InsertAfter(list[7], 77);
    exp_list = {-11, 0, 11, 22, 33, 44, 55, 66, 77, 88};
    EXPECT_TRUE(exp_list == list.ToVector());
  }
  {
    containers::BiDirectionalList<int> list;
    list.PushFront(1);
    list.InsertAfter(list.Front(), 2);
    list.InsertBefore(list[1], 3);
    list.InsertAfter(list[1], 9);
    std::vector<int> exp_list{1, 3, 9, 2};
    EXPECT_TRUE(exp_list == list.ToVector());
  }
}

TEST(Test_3, OperatorChecking) {
  containers::BiDirectionalList<int> list;
  list.PushBack(11);
  list.PushBack(22);
  list.PushBack(33);
  list.PushBack(55);
  EXPECT_TRUE(list[0]->value == 11);
  EXPECT_TRUE(list[2]->value == 33);
}

TEST(Test_4, EraserChecking) {
  {
    containers::BiDirectionalList<int> list;
    list.PushBack(11);
    list.PushBack(22);
    list.PushBack(33);
    list.PushBack(55);
    list.Erase(list[1]);
    std::vector<int> exp_list{11, 33, 55};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.Erase(list[2]);
    exp_list = {11, 33};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.Erase(list.Front());
    exp_list = {33};
    EXPECT_TRUE(exp_list == list.ToVector());
    list.Erase(list.Back());
    EXPECT_TRUE(list.IsEmpty());
  }
}

TEST(Test_4, FindChecking) {
  {
    containers::BiDirectionalList<int> list;
    std::vector<int> find_22{2, 5};
    std::vector<int> find_11{1, 6};
    std::vector<int> find_55{};
    EXPECT_TRUE(list.FindAll(22) == find_55);
    EXPECT_TRUE(list.FindAll(55) == find_55);
    EXPECT_TRUE(list.Find(55) == -1);
    EXPECT_TRUE(list.Find(0) == -1);
    list.PushBack(11);
    list.PushBack(22);
    list.PushBack(33);
    list.PushBack(44);
    list.PushFront(0);
    list.PushBack(22);
    list.PushBack(11);
    EXPECT_TRUE(list.Find(22) == 2);
    EXPECT_TRUE(list.Find(44) == 4);
    EXPECT_TRUE(list.Find(11) == 1);
    EXPECT_TRUE(list.FindAll(11) == find_11);
    EXPECT_TRUE(list.FindAll(22) == find_22);
    EXPECT_TRUE(list.FindAll(55) == find_55);
    EXPECT_TRUE(list.Find(55) == -1);
    EXPECT_TRUE(list.Find(0) == 0);
  }
}

TEST(Test_5, EqualityChecking) {
  containers::BiDirectionalList<int> list_1({1, 2, 3, 4});
  containers::BiDirectionalList<int> list_2;
  list_2.PushBack(2);
  EXPECT_TRUE(list_1 != list_2);
  list_2.PushFront(1);
  EXPECT_FALSE(list_1 == list_2);
  list_2.InsertAfter(list_2[1], 4);
  list_2.InsertBefore(list_2[2], 3);
  EXPECT_TRUE(list_1 == list_2);
}

TEST(Test_6, CopyOperators) {
  {
    containers::BiDirectionalList<int> list_1({1, 2, 3, 4, 5});
    containers::BiDirectionalList<int> list_2(list_1);
    EXPECT_TRUE(list_1 == list_2);
  }
  {
    containers::BiDirectionalList<int> list_1({1, 2, 3, 4, 5});
    containers::BiDirectionalList<int> list_2;
    list_2 = list_1;
    EXPECT_TRUE(list_1 == list_2);
  }
}

TEST(Test_7, MoveChecking) {
  containers::BiDirectionalList<int> list_1({1, 2, 3, 4, 5});
  containers::BiDirectionalList<int> list_2(std::move(list_1));
  EXPECT_FALSE(list_1 == list_2);
  EXPECT_TRUE(list_1.IsEmpty());
  EXPECT_EQ(list_2.Size(), 5);
}