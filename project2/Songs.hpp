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
class Song {
private:
// private, "normalized" values
  char _title[MAX_STRING_SIZE];
  char _artist[MAX_STRING_SIZE];
  unsigned int _minutes;
  unsigned int _seconds;
  char _album[MAX_STRING_SIZE];
  
  // public "consumable" values.
  
public: // I don't like having these internal values be public but I don't have
        // time to model it correctly. I need to quit my job.
  size_t index;
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
  Song& setTitle(const char*);
  Song& setArtist(const char*);
  Song& setMinutes(const unsigned int);
  Song& setSeconds(const unsigned int);
  Song& setAlbum(const char*);
  const char* getTitle() const;
  const char* getArtist() const;
  const unsigned int getMinutes();
  const unsigned int getSeconds();
  const char* getAlbum() const;
  void print() const;
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
