//
//  libian.hpp
//  project2
//
//  Created by Ian Harris on 07/06/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#ifndef libian_hpp
#define libian_hpp

#include <iostream>
#include <limits>

//#include "vecian.hpp"

// I don't like the size restriction for this, but it will have to do. Unless I
// just read in forever until a delimiter is found. But that would get super
// K&R, which I don't know if I want to do.
const int MAX_STRING_SIZE = 1000;

// Maximum amount of songs possible. I might try increasing this to 50k to mimic
// Google Play Music's capacity.
const int MAX_SONG_DB_SIZE = 2000;

void getString(char*);
int getInt(const int = std::numeric_limits<int>::lowest(),
           const int = std::numeric_limits<int>::max());

void normalize(char*);

#endif /* libian_hpp */
