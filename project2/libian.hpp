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
#include <cctype>

//#include "vecian.hpp"

// I don't like the size restriction for this, but it will have to do. Unless I
// just read in forever until a delimiter is found. But that would get super
// K&R, which I don't know if I want to do.
const int MAX_STRING_SIZE = 1000;

// Maximum amount of songs possible. I might try increasing this to 50k to mimic
// Google Play Music's capacity.

// So this is the reason I've been having so many problems. I can't prove it,
// but I'm definitely getting a stack overflow. I'm sure I'm running out of
// memory because everything is initiazlied, and it's taking up a crazy amount
// of memory. I really wish I could use strings. This is silly.
const int MAX_SONG_DB_SIZE = 1000;

// Maximum results that can be returned by a search. This is an arbitrary number
// and is a giant warning sign that my current memory functionality is abysmal.
// I don't have time to do anything about it!
const int MAX_RESULT_SIZE = 100;

void getString(char*);
int getInt(const int = std::numeric_limits<int>::min(),
           const int = std::numeric_limits<int>::max());

void normalize(char*);
bool isInt(const char*);

#endif /* libian_hpp */
