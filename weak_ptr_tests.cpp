#include <gtest/gtest.h>
#include "weak_ptr.h"
#include "shared_ptr.h"
#include <iostream>
#include <memory>

using pointers::WeakPtr;
using pointers::SharedPtr;

TEST(Test_0, Sample) {
  {
    int* p = new int(5);
    std::shared_ptr<int> sptr_1(p);
    std::shared_ptr<int> sptr_2 = sptr_1;
    std::weak_ptr<int> a(sptr_1);
    std::weak_ptr<int> b(a);
    EXPECT_TRUE(a.use_count() == 2);
    EXPECT_TRUE(b.use_count() == 2);
    std::shared_ptr<int> sptr_3 = a.lock();
    EXPECT_TRUE(a.use_count() == 3);
    EXPECT_TRUE(b.use_count() == 3);
    sptr_1.reset();
    sptr_2.reset();
    a.reset();
    EXPECT_FALSE(b.expired());
  }
}

TEST(Test_5, OneElementReseting) {
  {
    int* p = new int(5);
    SharedPtr<int> sptr_1(p);
    WeakPtr<int> a(sptr_1);
    sptr_1.Reset();
    EXPECT_TRUE(a.Expired());
    EXPECT_DEATH(*a.Lock(), ".*");
  }
}

TEST(Test_55, HereSomeoneDidNotDeleteEverything) {
  {
    int* p = new int(5);
    SharedPtr<int> sptr_1(p);
    SharedPtr<int> sptr_2 = sptr_1;
    WeakPtr<int> a(sptr_1);
    WeakPtr<int> b(a);
    EXPECT_TRUE(a.GetNumberOfConnectedShared() == 2);
    EXPECT_TRUE(b.GetNumberOfConnectedShared() == 2);
    SharedPtr<int> sptr_3 = a.Lock();
    EXPECT_TRUE(a.GetNumberOfConnectedShared() == 3);
    EXPECT_TRUE(b.GetNumberOfConnectedShared() == 3);
    sptr_1.Reset();
    sptr_2.Reset();
    a.Reset();
    EXPECT_FALSE(b.Expired());
  }
}

TEST(Test_1, WeakCreation) {
  int* p_int_1 = new int(1984);
  SharedPtr<int> sptr_1(p_int_1);
  SharedPtr<int> sptr_2(sptr_1);
  SharedPtr<int> sptr_3 = sptr_2;
  WeakPtr<int> wptr_1(sptr_1);
  EXPECT_TRUE(wptr_1.Get() == sptr_1.Get());
  EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 3);
  WeakPtr<int> wptr_2 = wptr_1;
  EXPECT_TRUE(wptr_2.Get() == sptr_1.Get());
  EXPECT_TRUE(wptr_2.GetNumberOfConnectedShared() == 3);
  sptr_3.Reset();
  EXPECT_TRUE(wptr_2.GetNumberOfConnectedShared() == 2);
  EXPECT_TRUE(wptr_2.GetNumberOfConnectedShared() == 2);
}

TEST(Test_2, DestructorChecking) {
  {
    int* p_int_1 = new int(1984);
    SharedPtr<int> sptr_1(p_int_1);
    WeakPtr<int> wptr_1(sptr_1);
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 1);
    sptr_1.Reset();
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 0);
    EXPECT_TRUE(wptr_1.Expired());
    EXPECT_TRUE(wptr_1.Get() != nullptr);
    wptr_1.Reset();
    EXPECT_TRUE(wptr_1.Get() == nullptr);
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 0);
  }
  {
    int* ptr = new int(228);
    SharedPtr<int> sptr_1(ptr);
    EXPECT_TRUE(ptr == sptr_1);
    WeakPtr<int> wptr_1(sptr_1);
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 1);
    SharedPtr<int> sptr_2 = sptr_1;
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 2);
    EXPECT_TRUE(sptr_2 == wptr_1.Lock());
    EXPECT_TRUE(sptr_2.GetCounter() == 2);
    SharedPtr<int> sptr_3 = wptr_1.Lock();
    EXPECT_TRUE(wptr_1.GetNumberOfConnectedShared() == 3);
    sptr_1.Reset();
    EXPECT_FALSE(wptr_1.Expired());
    sptr_2.Reset();
    sptr_3.Reset();
    EXPECT_TRUE(wptr_1.Expired());
  }
}

TEST(Test_4, LivingTest) {
  {
    WeakPtr<int> wp_0;
    {
      int* p = new int(8);
      SharedPtr<int> sp_2(p);
      SharedPtr<int> sp_1(sp_2);
      WeakPtr<int> wp_1(sp_1);
      SharedPtr<int> sp_3(sp_2);
      SharedPtr<int> sp_4(sp_3);
      WeakPtr<int> wp_2(sp_4);
      EXPECT_TRUE(wp_1.GetNumberOfConnectedShared() == 4);
      EXPECT_TRUE(wp_2.GetNumberOfConnectedShared() == 4);
      wp_0 = wp_2;
    }
    EXPECT_TRUE(wp_0.Expired());
  }
}

TEST(Test_6, OneMoreSingleElementResetting) {
  {
    int* ptr = new int(9);
    std::shared_ptr<int> pp(ptr);
    pp.reset();
    EXPECT_TRUE(*ptr == 9);
  }
  {
    int* ptr = new int(9);
    SharedPtr<int> pp(ptr);
    pp.Reset();
    EXPECT_TRUE(*ptr == 9);
  }
}

TEST(Test_7, NullptrCasesOneMoreTime) {
  {
    SharedPtr<int> sptr_1(nullptr);
    WeakPtr<int> w_1(sptr_1);
    EXPECT_TRUE(w_1.Expired());
    EXPECT_DEATH(w_1.Lock(), ".*");
    EXPECT_TRUE(w_1.Get() == nullptr);
    int* ptr_1 = new int(7);
    SharedPtr<int> sptr_2(ptr_1);
    sptr_1 = sptr_2;
    WeakPtr<int> w_2;
    w_2 = std::move(w_1);
    EXPECT_TRUE(w_2.GetNumberOfConnectedShared() == 0);
    EXPECT_TRUE(w_1.Expired());
    EXPECT_DEATH(w_2.Lock(), ".*");
  }
  {
    int* ptr_1 = new int(-300);
    SharedPtr<int> sptr_1(ptr_1);
    WeakPtr<int> w_1(sptr_1);
    SharedPtr<int> sptr_2;
    WeakPtr<int> w_2(sptr_2);
    w_1 = std::move(w_2);
    EXPECT_TRUE(w_1.Expired());
    EXPECT_TRUE(w_2.Expired());
  }
}

TEST(Test_8, DifficlutLifetemeTests) {
  {
    WeakPtr<int> w_0_outside;
    {
      WeakPtr<int> wp_0;
      {
        int* p = new int(8);
        SharedPtr<int> sp_2(p);
        SharedPtr<int> sp_1(sp_2);
        WeakPtr<int> wp_1(sp_1);
        SharedPtr<int> sp_3(sp_2);
        SharedPtr<int> sp_4(sp_3);
        WeakPtr<int> wp_2(sp_4);
        EXPECT_TRUE(wp_1.GetNumberOfConnectedShared() == 4);
        EXPECT_TRUE(wp_2.GetNumberOfConnectedShared() == 4);
        wp_0 = wp_2;
      }
      EXPECT_TRUE(wp_0.Expired());
      int* ptr = new int(4);
      SharedPtr<int> sptr_1_outside(ptr);
      WeakPtr<int> wp_1(sptr_1_outside);
      w_0_outside = wp_1;
    }
    EXPECT_TRUE(w_0_outside.Expired());
    int* ptr = new int(888);
    SharedPtr<int> sptr_1(ptr);
    WeakPtr<int> w_1_outside(sptr_1);
    w_0_outside = w_1_outside;
    EXPECT_TRUE(w_0_outside.GetNumberOfConnectedShared() == 1);
    EXPECT_TRUE(w_0_outside.Lock() == w_1_outside.Lock());
    {
      int* ptr = new int(777);
      SharedPtr<int> sptr_1(ptr);
      WeakPtr<int> w_0_inside(sptr_1);
      w_0_outside = std::move(w_0_inside);
      EXPECT_TRUE(w_0_outside.GetNumberOfConnectedShared() == 1);
      EXPECT_TRUE(*w_0_outside.Lock() == 777);
      EXPECT_TRUE(w_0_inside.Expired());
    }
    EXPECT_TRUE(w_0_outside.Expired());
    EXPECT_FALSE(w_1_outside.Expired());
    EXPECT_TRUE(w_1_outside.GetNumberOfConnectedShared() == 1);
    EXPECT_TRUE(*w_1_outside.Lock() == 888);
  }
  {
    WeakPtr<int> w_1;
    EXPECT_TRUE(w_1.Lock() == nullptr);
  }
}