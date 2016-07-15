//
//  Songs.cpp
//  project2
//
//  Created by Ian Harris on 07/08/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#include "Songs.hpp"

//Song::Song() {
//}

//MusicVal::MusicVal(const char name[]) {
//  strncpy(value, name, MAX_STRING_SIZE);
//  strncpy(_value, name, MAX_STRING_SIZE);
//  normalize(_value);
//}

char* MusicVal::operator()() const {
  return (char*)value;
}

void MusicVal::operator()(const char *val) {
  strncpy(value, val, MAX_STRING_SIZE);
  strncpy(_value, val, MAX_STRING_SIZE);
  normalize(_value);
}

unsigned int Time::operator()() const {
  return value;
}

void Time::operator()(const unsigned int val) {
  value = val;
}

bool Song::isPopulated() const {
  return populated;
}

bool Song::isPopulated(bool p) {
  return populated = p;
}

bool SongDB::print(const int index) const {
  if (!songs[index].isPopulated()) return false;
  std::cout
  << "Index: " << index << '\n'
  << "Title: " << songs[index].title() << '\n'
  << "Arist: " << songs[index].artist() << '\n'
  << "Duration: " << songs[index].minutes() << ':' << songs[index].seconds() << '\n'
  << "Album: " << songs[index].album() << "\n\n";
  return true;
}

bool SongDB::search(const char *query, Song results[], const Mode mode) const {
  // How do I break up the query? if the query is "   john ", it should
  // certainly trim the leading and trailing whitespace and then search for
  // "john" in whatever mode is specified. Should it match if it's a substring?
  // if it's an exact match? I don't have time to implement a real fuzzy string
  // matching algorithm, so (mostly) exact matches will have to suffice. Simply,
  // if the query is "JoHn," the case shouldn't matter. Already I have two
  // steps: trim leading and trailing whitespace, lowercase characters. This
  // naturally leads to other issues for more complex queries, however. If the
  // query is "   JdoG the     maGnifi  cent   ", should I trim, lowercase, and
  // then remove all superfluous whitespace, and then match? i.e., search for
  // exact match of "jdog the magnifi cent"? Should I splut up those words and
  // search for each one individually (which I don't have time for)? Should I
  // remove all whitespace and search for "jdogthemagnificent"? This would also
  // require whatever I do to the query to be done to each song in the db.
  
  // n.b., the above sentence may lead to having an additional field in each
  // Song object which holds the "search" string representing the fields which
  // are lowercase with whitespace (and potentially all non-alnum chars)
  // stripped, which would mean I would only need to generate that value once,
  // which keeps searching reasonably efficient.
  
  // Either way, clearly the searching for the assignment specifications will be
  // rather barebones and not as nice as full-featured as I would like. Alas,
  // there are only so many hours in the day.
  switch (mode) {
    case TITLE:
      // search for title
      break;
      
    case ARTIST:
      // search for artist
      break;
      
    case TIME:
      // search for time. this will be fun!
      break;
      
    case ALBUM:
      // search for album
      break;
      
    case OTHER:
      // search for everything
      break;
      
    default:
      // unknown search mode.
      break;
  }
  return false;
}

SongDB::~SongDB() {
  std::ofstream
  f("songs.txt");
  for (auto s : songs) {
    // quit on first unpopulated entry.
    if (!s.isPopulated()) break;
    f << s.title() << ';'
    << s.artist() << ';'
    << s.minutes() << ';'
    << s.seconds() << ';'
    << s.album() << ";\n";
  }
  f.close();
  
  std::cout << "\nThanks for using Ian's music manager!\n";
}

Mode strToMode(const char* mode) {
  char lower[10];
  for (size_t i = 0; i < strlen(mode); i++) {
    lower[i] = tolower(mode[i]);
  }
  if (strncmp("title", lower, strlen(lower)) == 0) return TITLE;
  if (strncmp("artist", lower, strlen(lower)) == 0) return ARTIST;
  if (strncmp("time", lower, strlen(lower)) == 0) return TIME;
  if (strncmp("album", lower, strlen(lower)) == 0) return ALBUM;
  return OTHER; // This is necessary for the compiler with how the above series
  // of returns are structured, but it will never be reached.
}
