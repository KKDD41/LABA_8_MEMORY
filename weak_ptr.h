#ifndef WEAK_PTR_H_
#define WEAK_PTR_H_
#include "shared_ptr.h"
#include <cassert>

namespace pointers {

template<typename T>
class WeakPtr {
 public:
  // CONSTRUCTORS
  WeakPtr() = default;
  explicit WeakPtr(const SharedPtr<T>& shared_ptr);
  WeakPtr(const WeakPtr<T>& other_weak_pointer);
  WeakPtr(WeakPtr<T>&& another_ptr) noexcept;

  // DELETING FUNCTIONS
  ~WeakPtr();
  void Reset();

  // ASSIGMENT OPERATORS
  WeakPtr<T>& operator=(const WeakPtr<T>& rhs);
  WeakPtr<T>& operator=(WeakPtr<T>&& rhs) noexcept;

  // INNER POINTER PROCESSING
  bool Expired() const;
  SharedPtr<T> Lock();

  // GETTERS
  T* Get() {
    return weak_inner_pointer_;
  }
  const T* Get() const {
    return weak_inner_pointer_;
  }
  int GetNumberOfConnectedShared() {
    if (get_ == nullptr) {
      return 0;
    }
    return (get_->counter_of_smart_ptr_);
  }

 private:
  T* weak_inner_pointer_{nullptr};
  CounterManager* get_ = nullptr;
};

// CONSTRUCTORS
template<typename T>
WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared_ptr) {
  weak_inner_pointer_ = shared_ptr.inner_pointer_;
  get_ = shared_ptr.get_;
  if (get_ != nullptr) {
    (get_->weak_validity_)++;
  }
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& another_ptr) noexcept {
  (*this) = std::move(another_ptr);
}
template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other_weak_pointer) {
  (*this) = other_weak_pointer;
}

// ASSIGMENT OPERATORS
template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& rhs) {
  if (this == &rhs) {
    return (*this);
  }
  this->Reset();
  weak_inner_pointer_ = rhs.weak_inner_pointer_;
  get_ = rhs.get_;
  if (get_ != nullptr) {
    (get_->weak_validity_)++;
  }
  return (*this);
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& rhs) noexcept {
  if (this == &rhs) {
    return (*this);
  }
  this->Reset();
  weak_inner_pointer_ = std::move(rhs.weak_inner_pointer_);
  get_ = std::move(rhs.get_);
  rhs.weak_inner_pointer_ = nullptr;
  rhs.get_ = nullptr;
  return (*this);
}

// DESTRUCTORS
template<typename T>
WeakPtr<T>::~WeakPtr() {
  if (get_ != nullptr) {
    (get_->weak_validity_)--;
    if (!get_->IsConnectedToPtr()) {
      delete get_;
    }
  }
  weak_inner_pointer_ = nullptr;
  get_ = nullptr;
}

template<typename T>
void WeakPtr<T>::Reset() {
  if (get_ != nullptr) {
    (get_->weak_validity_)--;
    if (!get_->IsConnectedToPtr()) {
      delete get_;
    }
  }
  weak_inner_pointer_ = nullptr;
  get_ = nullptr;
}

// INNER POINTER PROCESSING
template<typename T>
SharedPtr<T> WeakPtr<T>::Lock() {
  SharedPtr<T> shared_ptr{};
  if (weak_inner_pointer_ == nullptr) {
    return shared_ptr;
  }
  assert(!this->Expired());
  shared_ptr.inner_pointer_ = weak_inner_pointer_;
  shared_ptr.get_ = get_;
  if (get_ != nullptr) {
    (get_->counter_of_smart_ptr_)++;
  }
  return shared_ptr;
}

template<typename T>
bool WeakPtr<T>::Expired() const {
  if (get_ == nullptr || get_->counter_of_smart_ptr_ == 0 ||
      weak_inner_pointer_ == nullptr) {
    return true;
  }
  return false;
}


}  // namespace pointers

#endif  // WEAK_PTR_H_
