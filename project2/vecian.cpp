//
//  vecian.cpp
//  project2
//
//  Created by Ian Harris on 07/06/16.
//  Copyright © 2016 Ian Harris. All rights reserved.

// Apparently these need to be defined elsewhere, but I don't really know.
// Bleh.

#include "vecian.hpp"

//template <typename T>
//class Vecian {
//  size_t size;
//  T items[];
//public:
//  // returns reference to element at pos with bounds checking
//  T& at(size_t pos) const;
//  // returns reference to element at pos without bounds checking
//  T& operator[](size_t pos) const;
//  // replaces the contents of the container
//  Vecian& operator=(const Vecian& other);
//  // replaces the contents of the container
//  void assign(size_t count, const T& value);
//};

template <typename T>
T& Vecian<T>::at(size_t pos) const {
  if (pos >= this->size) {
    std::cerr << "Vecian: .at access out of bounds!\n";
    exit(1); // XXX: Define error numbering properly
  }
  return this->items[pos];
}

template <typename T>
T& Vecian<T>::operator[](size_t pos) const {
  return this->items[pos];
}

template <typename T>
void Vecian<T>::clear() {
  free(this);
  this = (Vecian<T> *) malloc(1);
  if (this == nullptr) {
    std::cerr << "Error allocating new object!\n";
    exit(1); // XXX: Define error numbering properly
  }
}

template <typename T>
void Vecian<T>::push_back(const T& value) {
  if (_size >= _max_size - 1) {
    std::cerr << "Error expanding Vecian!\n";
    exit(1); // XXX: Define error numbering properly
  }
  if (_size >= _capacity) {
    _capacity *= 2;
    this = (Vecian<T> *) realloc(this, _capacity * sizeof this);
    if (this == nullptr) {
      std::cerr << "Error expanding Vecian!\n";
      exit(1); // XXX: Define error numbering properly
    }
  }
  _items[_size++] = value;
}
