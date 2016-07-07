//
//  vecian.hpp
//  project2
//
//  Created by Ian Harris on 07/06/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#ifndef vecian_hpp
#define vecian_hpp

#include <cstddef>
#include <iostream>

// This number is (currently) arbitrary. For some reason I remember reading that
// an exponential increase of size for a vector's size is typically a good idea,
// but I don't know what a reasonable starting value would be. 64 actually still
// seems relatively large, but maybe it's fine.
const int VECIAN_START_SIZE = 64;
// Completely arbitrary max size which will probably be removed.
const int VECIAN_MAX_SIZE = 50000;

// Templated vec class that internally stores the items in an array.
template <typename T>
class Vecian {
  // Current number of elements in the container.
  size_t _size;
  // Current number of elements the container has currently allocated space for.
  size_t _capacity;
  // Maximum possible size of container.
  size_t _max_size;
  T _items[];
public:
  // Returns reference to element at pos with bounds checking.
  T& at(size_t pos) const;
  // Returns reference to element at pos without bounds checking.
  T& operator[](size_t pos) const;
  // Replaces the contents of the container.
  Vecian& operator=(const Vecian& other);
  // Replaces the contents of the container.
  void assign(size_t count, const T& value);
  // Removes all elements from the container.
  void clear();
  // Appends the given element value to the end of the container.
  void push_back(const T& value);
  // Returns the number of elements in the container.
  size_t size() const;
  // Returns the number of elements that the container has currently allocated
  // space for.
  size_t capacity() const;
};
#endif /* vecian_hpp */
