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

enum Mode {
  TITLE,
  ARTIST,
  TIME,
  ALBUM,
  OTHER
};

class Song {
private:
// private, "normalized" values
  char *norm_title;
  char *norm_artist;
  char *norm_album;
  char *title;
  char *artist;
  unsigned int minutes; // Although it's highly unlikely, using an int instead
  // of a char allows for songs longer than 255 minutes.
  unsigned int seconds; // Seconds are forced into a range of 0-59, which means
  // the smallest type to contain it is an unsigned char.
  // Unforunately I don't care to figure out why casting
  // to an unsigned char in some logic for reading in from
  // a file is causing issues, so I'm just using an
  // unsigned int instead. No system running this program
  // will actually have constrained memory usage.
  char *album;
  
// public "consumable" values.
public: // I don't like having these internal values be public but I don't have
        // time to model it correctly. I need to quit my job.
  size_t index;
  bool isPopulated;
  Song& setTitle(const char*);
  Song& setArtist(const char*);
  Song& setMinutes(const unsigned int);
  Song& setSeconds(const unsigned int);
  Song& setAlbum(const char*);
  const char* getTitle() const;
  const char* getNormTitle() const;
  const char* getArtist() const;
  const char* getNormArtist() const;
  unsigned int getMinutes() const;
  unsigned int getSeconds() const;
  const char* getAlbum() const;
  const char* getNormAlbum() const;
  void print() const;
  ~Song();
};

struct SongDB {
  int items;
  Song *songs;
  bool print(const int) const;
  bool search(const char*, Song[], const Mode = OTHER) const;
  SongDB();
  ~SongDB();
};

Mode strToMode(const char*);

#endif /* Songs_hpp */
