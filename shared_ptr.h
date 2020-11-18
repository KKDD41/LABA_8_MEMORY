#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include <iostream>
#include <ostream>
#include <cassert>

namespace pointers {

template<typename T>
class WeakPtr;

class CounterManager {
 public:
  CounterManager() = default;
  ~CounterManager() = default;

  bool IsConnectedToPtr() const {
    return counter_of_smart_ptr_ + weak_validity_ > 0;
  }

  CounterManager& operator=(const CounterManager& rhs) {
    counter_of_smart_ptr_ = rhs.counter_of_smart_ptr_;
    weak_validity_ = rhs.weak_validity_;
    return *this;
  }
  CounterManager& operator=(CounterManager&& rhs) noexcept {
    this->counter_of_smart_ptr_ = rhs.counter_of_smart_ptr_;
    this->weak_validity_ = rhs.weak_validity_;
    rhs.counter_of_smart_ptr_ = 0;
    rhs.weak_validity_ = 0;
    return *this;
  }
  bool operator==(const CounterManager& rhs) const {
    return (this->counter_of_smart_ptr_ == rhs.counter_of_smart_ptr_ &&
            this->weak_validity_ == rhs.weak_validity_);
  }

  int counter_of_smart_ptr_ = 0;
  int weak_validity_ = 0;
};

template<typename T>
class SharedPtr {
 public:
  friend class WeakPtr<T>;

  // CONSTRUCTORS
  SharedPtr() = default;
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr<T>& other_pointer);
  SharedPtr(SharedPtr<T>&& other_pointer) noexcept;

  // DELETING OPERATORS
  ~SharedPtr();
  void Reset();

  // ASSIGMENT OPERATORS
  SharedPtr& operator=(const SharedPtr<T>& rhs);
  SharedPtr& operator=(SharedPtr<T>&& rhs) noexcept;

  // GETTERS
  int GetCounter() const {
    return (get_ != nullptr) ? (get_->counter_of_smart_ptr_) : 0;
  }

  T* Get() {
    return inner_pointer_;
  }
  const T* Get() const {
    return inner_pointer_;
  }

  T& operator*();
  const T& operator*() const;

  T* operator->();
  const T* operator->() const;

  // COMPARING OPERATORS
  bool operator==(const T* rhs) const;
  bool operator==(const SharedPtr<T>& rhs) const;

  bool operator!=(const SharedPtr<T>& rhs) const;
  bool operator!=(const T* rhs) const;

  friend bool operator==(const T* lhs, const SharedPtr<T> rhs) {
    return lhs == rhs.inner_pointer_;
  }
  friend bool operator!=(const T* lhs, const SharedPtr<T> rhs) {
    return rhs != lhs;
  }

 private:
  T* inner_pointer_{nullptr};
  CounterManager* get_{};
};

// CONSTRUCTORS

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr) {
  inner_pointer_ = ptr;
  if (ptr != nullptr) {
    get_ = new CounterManager();
    get_->counter_of_smart_ptr_ = 1;
  }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other_pointer) {
  (*this) = other_pointer;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other_pointer) noexcept {
  (*this) = std::move(other_pointer);
}

// DELETING OPERATORS

template<typename T>
SharedPtr<T>::~SharedPtr() {
  if (get_ == nullptr) {
    return;
  }
  (get_->counter_of_smart_ptr_)--;
  if (get_->counter_of_smart_ptr_ == 0) {
    delete inner_pointer_;
    if (!get_->IsConnectedToPtr()) {
      delete get_;
    }
    get_ = nullptr;
    inner_pointer_ = nullptr;
  } else {
    get_ = nullptr;
    inner_pointer_ = nullptr;
  }
}

template<typename T>
void SharedPtr<T>::Reset() {
  if (get_ == nullptr) {
    return;
  }
  (get_->counter_of_smart_ptr_)--;
  if (get_->counter_of_smart_ptr_ == 0) {
    delete inner_pointer_;
    if (!get_->IsConnectedToPtr()) {
      delete get_;
    }
  }
  get_ = nullptr;
  inner_pointer_ = nullptr;
}

// ASSIGMENT OPERATORS

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->Reset();
  inner_pointer_ = rhs.inner_pointer_;
  get_ = rhs.get_;
  if (rhs.get_ != nullptr) {
    (get_->counter_of_smart_ptr_)++;
  }
  return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }
  if (rhs.get_ != nullptr && inner_pointer_ == rhs.inner_pointer_) {
    (rhs.get_->counter_of_smart_ptr_)--;
    rhs.inner_pointer_ = nullptr;
    rhs.get_ = nullptr;
    return *this;
  }
  this->Reset();
  inner_pointer_ = std::move(rhs.inner_pointer_);
  get_ = std::move(rhs.get_);
  rhs.inner_pointer_ = nullptr;
  rhs.get_ = nullptr;
  return *this;
}

// COMPARING OPERATORS

template<typename T>
bool SharedPtr<T>::operator==(const T* rhs) const {
  return (inner_pointer_ == rhs);
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& rhs) const {
  if (inner_pointer_ == rhs.inner_pointer_ && get_ == rhs.get_) {
    return true;
  }
  return false;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T>& rhs) const {
  return !((*this) == rhs);
}

template<typename T>
bool SharedPtr<T>::operator!=(const T* rhs) const {
  return inner_pointer_ != rhs;
}

// GETTERS

template<typename T>
T& SharedPtr<T>::operator*() {
  assert(inner_pointer_ != nullptr);
  return *inner_pointer_;
}

template<typename T>
const T& SharedPtr<T>::operator*() const {
  assert(inner_pointer_ != nullptr);
  return *inner_pointer_;
}

template<typename T>
T* SharedPtr<T>::operator->() {
  return inner_pointer_;
}

template<typename T>
const T* SharedPtr<T>::operator->() const {
  return inner_pointer_;
}

}  // namespace pointers

#endif  // SHARED_PTR_H_
