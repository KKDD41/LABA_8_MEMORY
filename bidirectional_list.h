#ifndef BIDIRECTIONAL_LIST_H_
#define BIDIRECTIONAL_LIST_H_

#include <iostream>
#include <vector>
#include <cassert>

namespace containers {

template<typename T>
class BiDirectionalList {
 public:
  struct Node {
   public:
    friend class BiDirectionalList<T>;

    Node() = delete;
    T value;

   private:
    Node(Node* prev, Node* next, T&& passed_value) :
      value(std::move(passed_value)), prev_{prev}, next_{next} {}
    Node* prev_;
    Node* next_;
  };

  // CONSTRUCTORS
  BiDirectionalList() = default;
  BiDirectionalList(const std::initializer_list<T>& init_list);
  BiDirectionalList<T>(BiDirectionalList<T>&& rhs) noexcept {
    (*this) = std::move(rhs);
  }
  BiDirectionalList<T>(const BiDirectionalList<T>& rhs) {
    (*this) = rhs;
  }
  ~BiDirectionalList() {
    this->DeleteAll();
  }

  // GETTERS
  int Size() const;
  bool IsEmpty() const;
  std::vector<T> ToVector() const;
  Node* Front() {
    return front_;
  }
  const Node* Front() const {
    return front_;
  }
  Node* Back() {
    return back_;
  }
  const Node* Back() const {
    return back_;
  }

  // SEARCHING IN LIST
  int Find(const T& value) const;
  std::vector<int> FindAll(const T& value) const;

  Node* operator[](const int& index);
  const Node* operator[](const int& index) const;

  // DELETING METHODS
  void PopBack();
  void PopFront();
  void Erase(Node* element);
  void DeleteAll();

  // ADDING METHODS WITH (CONST T&)
  void PushFront(const T& value) {
    PushFrontCopy(value);
  }
  void PushBack(const T& value) {
    PushBackCopy(value);
  }
  void InsertBefore(Node* element, const T& value) {
    InsertBeforeCopy(element, value);
  }
  void InsertAfter(Node* element, const T& value) {
    InsertAfterCopy(element, value);
  }

  // ADDING METHODS WITH (T&&)
  void PushFront(T&& value) {
    PushFrontCopy(std::move(value));
  }
  void PushBack(T&& value) {
    PushBackCopy(std::move(value));
  }
  void InsertBefore(Node* element, T&& value) {
    InsertBeforeCopy(element, std::move(value));
  }
  void InsertAfter(Node* element, T&& value) {
    InsertAfterCopy(element, std::move(value));
  }

  // COMPARING OPERATORS
  bool operator==(const BiDirectionalList<T>& rhs) const;
  bool operator!=(const BiDirectionalList<T>& rhs) const;

  // ASSIGNMENT OPERATORS
  BiDirectionalList<T>& operator=(const BiDirectionalList<T>& rhs);
  BiDirectionalList<T>& operator=(BiDirectionalList<T>&& rhs) noexcept;

 private:
  Node* front_{nullptr};
  Node* back_{nullptr};
  int size_ = 0;

  // Functions, similar to their methods,
  // that work with a copy of the passed object and avoid code duplication.
  // To be honest, this trick was suggested to me.
  void PushFrontCopy(T value);
  void PushBackCopy(T value);
  void InsertBeforeCopy(Node* element, T value);
  void InsertAfterCopy(Node* element, T value);
};

// GETTERS
template<typename T>
int BiDirectionalList<T>::Size() const {
  return size_;
}

template<typename T>
bool BiDirectionalList<T>::IsEmpty() const {
  return size_ == 0;
}

template<typename T>
std::vector<T> BiDirectionalList<T>::ToVector() const {
  std::vector<T> vector_of_values;
  if (size_ == 0) {
    return vector_of_values;
  }
  Node* tmp = front_;
  vector_of_values.push_back(front_->value);
  for (int i = 1; i < size_; ++i) {
    tmp = tmp->next_;
    vector_of_values.push_back(tmp->value);
  }
  return vector_of_values;
}

template<typename T>
int BiDirectionalList<T>::Find(const T& value) const {
  if (this->IsEmpty()) {
    return -1;
  }
  Node* tmp = front_;
  for (int i = 0; i < size_; i++) {
    if (tmp->value == value) {
      return i;
    }
    tmp = tmp->next_;
  }
  return -1;
}

template<typename T>
std::vector<int> BiDirectionalList<T>::FindAll(const T& value) const {
  std::vector<int> indexes_with_value;
  Node* tmp = front_;
  for (int i = 0; i < size_; i++) {
    if (tmp->value == value) {
      indexes_with_value.push_back(i);
    }
    tmp = tmp->next_;
  }
  return indexes_with_value;
}

template<typename T>
BiDirectionalList<T>::BiDirectionalList(const std::initializer_list<T>&
init_list) {
  for (auto& value : init_list) {
    this->PushBack(value);
  }
}

template<typename T>
void BiDirectionalList<T>::PopBack() {
  assert(size_ > 0);
  if (size_ == 1) {
    delete back_;
    front_ = nullptr;
    back_ = nullptr;
    size_--;
    return;
  }
  Node* tmp = back_->prev_;
  delete back_;
  back_ = tmp;
  back_->next_ = nullptr;
  --size_;
}

template<typename T>
void BiDirectionalList<T>::PopFront() {
  assert(size_ > 0);
  if (size_ == 1) {
    delete front_;
    front_ = nullptr;
    back_ = nullptr;
    size_--;
    return;
  }
  Node* tmp = front_->next_;
  delete front_;
  front_ = tmp;
  front_->prev_ = nullptr;
  --size_;
}
template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList::Node* element) {
  assert(size_ > 0 || element != nullptr);
  Node* before_element = element->prev_;
  Node* after_element = element->next_;
  if (before_element != nullptr) {
    before_element->next_ = after_element;
  } else {
    front_ = after_element;
  }
  if (after_element != nullptr) {
    after_element->prev_ = before_element;
  } else {
    back_ = before_element;
  }
  delete element;
  size_--;
}

template<typename T>
typename BiDirectionalList<T>::Node* BiDirectionalList<T>::operator[](
    const int& index) {
  assert(index >= 0 && index < size_);
  Node* tmp = front_;
  int counter = 0;
  while (tmp != nullptr) {
    if (counter == index) {
      return tmp;
    }
    tmp = tmp->next_;
    counter++;
  }
  return nullptr;
}

template<typename T>
const typename BiDirectionalList<T>::Node* BiDirectionalList<T>::operator[](
    const int& index) const {
  assert(index >= 0 && index < size_);
  Node* tmp = front_;
  int counter = 0;
  while (tmp != nullptr) {
    if (counter == index) {
      return tmp;
    }
    tmp = tmp->next_;
    counter++;
  }
  return nullptr;
}
template<typename T>
bool BiDirectionalList<T>::operator==(const BiDirectionalList<T>& rhs) const {
  if ((*this).size_ != rhs.size_) {
    return false;
  }
  for (int i = 0; i < size_; ++i) {
    if ((*this)[i]->value != rhs[i]->value) {
      return false;
    }
  }
  return true;
}

template<typename T>
bool BiDirectionalList<T>::operator!=(const BiDirectionalList<T>& rhs) const {
  return !(*this == rhs);
}

template<typename T>
BiDirectionalList<T>&
    BiDirectionalList<T>::operator=(const BiDirectionalList<T>& rhs) {
  if (this == &rhs) {
    return *this;
  }
  this->DeleteAll();
  Node* tmp = rhs.front_;
  while (tmp != nullptr) {
    this->PushBack(tmp->value);
    tmp = tmp->next_;
  }
  size_ = rhs.size_;
  return *this;
}

template<typename T>
BiDirectionalList<T>&
    BiDirectionalList<T>::operator=(BiDirectionalList<T>&&rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }
  this->DeleteAll();
  size_ = rhs.size_;
  back_ = rhs.back_;
  front_ = rhs.front_;
  rhs.size_ = 0;
  rhs.back_ = nullptr;
  rhs.front_ = nullptr;
  return *this;
}

template<typename T>
void BiDirectionalList<T>::DeleteAll() {
  Node* tmp = front_;
  Node* tmp2 = tmp;
  while (tmp2 != nullptr) {
    tmp2 = tmp->next_;
    delete tmp;
    tmp = tmp2;
  }
  front_ = nullptr;
  back_ = nullptr;
  size_ = 0;
}
template<typename T>
void BiDirectionalList<T>::PushFrontCopy(T value) {
  Node* value_in_list = new Node(nullptr, front_, std::move(value));
  if (size_ == 0) {
    front_ = value_in_list;
    back_ = value_in_list;
    size_++;
    return;
  }
  front_->prev_ = value_in_list;
  front_ = value_in_list;
  size_++;
}
template<typename T>
void BiDirectionalList<T>::PushBackCopy(T value) {
  Node* value_in_list = new Node(back_, nullptr, std::move(value));
  if (size_ == 0) {
    front_ = value_in_list;
    back_ = value_in_list;
    size_++;
    return;
  }
  back_->next_ = value_in_list;
  back_ = value_in_list;
  size_++;
}
template<typename T>
void BiDirectionalList<T>::InsertBeforeCopy(BiDirectionalList::Node* element,
                                            T value) {
  assert(size_ > 0 && element != nullptr);
  Node* node_before_element = new Node(element->prev_,
                                       element, std::move(value));
  if (element->prev_ == nullptr) {
    front_ = node_before_element;
  } else {
    Node* tmp = element->prev_;
    tmp->next_ = node_before_element;
  }
  element->prev_ = node_before_element;
  size_++;
}
template<typename T>
void BiDirectionalList<T>::InsertAfterCopy(BiDirectionalList::Node* element,
                                           T value) {
  assert(size_ > 0 && element != nullptr);
  Node* node_after_element = new Node(element, element->next_,
                                      std::move(value));
  if (element->next_ == nullptr) {
    back_ = node_after_element;
  } else {
    Node* tmp = element->next_;
    tmp->prev_ = node_after_element;
  }
  element->next_ = node_after_element;
  size_++;
}

}  // namespace containers


#endif  // BIDIRECTIONAL_LIST_H_
