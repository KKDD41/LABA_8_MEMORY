#include <iostream>
#include <gtest/gtest.h>
#include "shared_ptr.h"

using pointers::SharedPtr;

TEST(Test_0, ConstructorsAndGetters) {
  int* a = new int(42);
  pointers::SharedPtr<int> sptr_on_a_1(a);
  pointers::SharedPtr<int> sptr_on_a_2(sptr_on_a_1);
  EXPECT_TRUE(sptr_on_a_1.GetCounter() == 2);
  EXPECT_TRUE(sptr_on_a_2.GetCounter() == 2);
  pointers::SharedPtr<int> sptr_on_a_3(sptr_on_a_2);
  EXPECT_TRUE(sptr_on_a_1.GetCounter() == 3);
  EXPECT_TRUE(sptr_on_a_2.GetCounter() == 3);
  EXPECT_TRUE(sptr_on_a_3.GetCounter() == 3);
  pointers::SharedPtr<int> sptr_on_a_4(std::move(sptr_on_a_3));
  EXPECT_TRUE(sptr_on_a_3.GetCounter() == 0 &&
                       sptr_on_a_3.Get() == nullptr);
  EXPECT_TRUE(sptr_on_a_4.GetCounter() == 3);
  EXPECT_TRUE(sptr_on_a_1.GetCounter() == 3);
  EXPECT_TRUE(sptr_on_a_2.GetCounter() == 3);
}

TEST(Test_1, DestructorChecking) {
  {
    int* a = new int(42);
    pointers::SharedPtr<int> sptr_on_a_1(a);
    pointers::SharedPtr<int> sptr_on_a_2 = sptr_on_a_1;
    EXPECT_EQ(*sptr_on_a_2, 42);
    EXPECT_EQ(*sptr_on_a_1, 42);
    *sptr_on_a_1 = 1000;
    sptr_on_a_1.~SharedPtr();
    EXPECT_DEATH(*sptr_on_a_1, ".*");
    EXPECT_TRUE(sptr_on_a_1.Get() == nullptr);
    EXPECT_EQ(*sptr_on_a_2, 1000);
    EXPECT_EQ(sptr_on_a_2.GetCounter(), 1);
    pointers::SharedPtr<int> sptr_on_a_3 = std::move(sptr_on_a_2);
    EXPECT_EQ(*sptr_on_a_3, 1000);
    EXPECT_DEATH(*sptr_on_a_2, ".*");
    EXPECT_TRUE(sptr_on_a_2.Get() == nullptr);
  }
}

TEST(Test_3, EqualityTesting) {
  {
    int* ptr = new int(42);
    int* ptr_2 = new int(13);
    pointers::SharedPtr<int> one(ptr);
    EXPECT_TRUE(one == one);
    one = one;
    EXPECT_TRUE(one == one);
    EXPECT_TRUE(one.GetCounter() == 1);
    pointers::SharedPtr<int> two(ptr_2);
    one = two;
    EXPECT_TRUE(one == two);
    EXPECT_TRUE(two == one);
    EXPECT_TRUE(&one != &two);
    EXPECT_TRUE(one.GetCounter() == 2);
    EXPECT_TRUE(two.GetCounter() == 2);
    EXPECT_TRUE(one.Get() == two.Get());
    EXPECT_FALSE(two == nullptr);
    one = std::move(two);
    EXPECT_TRUE(two == nullptr);
    EXPECT_TRUE(one.GetCounter() == 1);
  }
  {
    SharedPtr<int> p_1;
    SharedPtr<int> p_2;
    p_2 = p_1;
    EXPECT_TRUE(p_1 == p_2);
    EXPECT_FALSE(p_1 != p_2);
    int* regular_p_1 = new int(3);
    SharedPtr<int> p_3(regular_p_1);
    EXPECT_FALSE(p_1 == p_3);
    EXPECT_TRUE(p_3 != p_2);
    p_2 = std::move(p_3);
    EXPECT_TRUE(p_2.GetCounter() == 1);
    EXPECT_TRUE(p_1.GetCounter() == 0);
    EXPECT_FALSE(p_1 == p_2);
    EXPECT_TRUE(p_1 == p_3);
    EXPECT_DEATH(*p_3, ".*");
    p_1 = p_2;
    EXPECT_TRUE(p_1.GetCounter() == 2);
    EXPECT_TRUE(p_2.GetCounter() == 2);
    EXPECT_TRUE(*p_2 == 3);
    p_1.Reset();
    EXPECT_TRUE(p_1.GetCounter() == 0);
    EXPECT_DEATH(*p_1, ".*");
    EXPECT_TRUE(p_2.GetCounter() == 1);
    p_2.Reset();
    EXPECT_TRUE(p_1 == p_2);
    EXPECT_TRUE(p_2.GetCounter() == 0);
    EXPECT_TRUE(p_2 == p_1 && p_2 == p_3);
  }
}

TEST(Test_4, ResetChecking) {
  pointers::SharedPtr<int> pointer;
  pointer.Reset();
  EXPECT_TRUE(pointer.GetCounter() == 0);
  EXPECT_TRUE(pointer.Get() == nullptr);
  int* p = new int(6);
  pointers::SharedPtr<int> pointer_1(p);
  pointer = pointer_1;
  EXPECT_TRUE(pointer.GetCounter() == 2);
  EXPECT_TRUE(pointer.Get() == pointer_1.Get());
  pointer_1.Reset();
  EXPECT_TRUE(pointer_1.GetCounter() == 0);
  EXPECT_TRUE(pointer.GetCounter() == 1);
  EXPECT_TRUE(pointer_1.Get() == nullptr);
  pointer.Reset();
  EXPECT_TRUE(pointer.GetCounter() == 0);
  EXPECT_TRUE(pointer.Get() == nullptr);
}

