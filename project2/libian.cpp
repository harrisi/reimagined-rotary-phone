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