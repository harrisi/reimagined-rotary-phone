//
//  libian.cpp
//  project2
//
//  Created by Ian Harris on 07/06/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#include "libian.hpp"

void getString(char *buf) {
  while(!std::cin.get(buf, MAX_STRING_SIZE - 1)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input! Try again: ";
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInt(const int min, const int max) {
  int res;
  while (!(std::cin >> res) || res < min || res > max) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input! Try again: ";
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return res;
}

void lowercase(char *val) {
  char *i = val;
  while (*i) {
    *i = tolower(*i);
    i++;
  }
}

// I don't really like the naming, but whatever.
void normalize(char *val) {
  char *i = val;
  char *j = val;
  while (*j != '\0') {
    *i = tolower(*j++);
    if (!isspace(*i)) {
      i++;
    }
  }
  *i = '\0';
}

bool isInt(const char* val) {
  while (*val) {
    if(!isdigit(*val++)) {
      return false;
    }
  }
  return true;
}

// Returns the length of the larger string
size_t maxlen(const char *a, const char *b) {
  return (strlen(a) >= strlen(b) ? strlen(a) : strlen(b));
}