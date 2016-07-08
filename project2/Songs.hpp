//
//  Songs.hpp
//  project2
//
//  Created by Ian Harris on 07/08/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#ifndef Songs_hpp
#define Songs_hpp

#include <iostream>
#include <fstream>
#include <cstring>
#include "libian.hpp"

// Move to Parser.
enum Mode {
  TITLE,
  ARTIST,
  TIME,
  ALBUM,
  OTHER // This won't be used, but I'm reserving it for now.
};

// private is best, but I don't want to deal with that right now.
struct Song {
  // I could model this object better and define a destructor that would do all
  // the cleanup for me for free. This would probably be my favorite approach,
  // but it would require remodelling the whole program flow, most likely. For
  // now I will be much more rudimentary.
  char title[MAX_STRING_SIZE];
  char artist[MAX_STRING_SIZE];
  unsigned int minutes; // Although it's highly unlikely, using an int instead
  // of a char allows for songs longer than 255 minutes.
  unsigned int seconds; // Seconds are forced into a range of 0-59, which means
  // the smallest type to contain it is an unsigned char.
  // Unforunately I don't care to figure out why casting to an unsigned char
  // in some logic for reading in from a file is causing issues, so I'm just
  // using an unsigned int instead. No system running this program will actually
  // have very constrained memory usage or disk space.
  char album[MAX_STRING_SIZE];
  bool isPopulated;
};

struct SongDB {
  int items;
  Song songs[MAX_SONG_DB_SIZE];
  bool print(const int) const;
  bool search(const char*, Song[], const Mode = OTHER) const;
  ~SongDB();
};

Mode strToMode(const char*);

#endif /* Songs_hpp */
