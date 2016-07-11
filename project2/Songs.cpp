//
//  Songs.cpp
//  project2
//
//  Created by Ian Harris on 07/08/16.
//  Copyright © 2016 Ian Harris. All rights reserved.
//

#include <iomanip>
#include "Songs.hpp"

const char* Song::getTitle() const {
  return _title;
}

const char* Song::getArtist() const {
  return _artist;
}

const unsigned int Song::getMinutes() {
  return _minutes;
}

const unsigned int Song::getSeconds() {
  return _seconds;
}

const char* Song::getAlbum() const {
  return _album;
}

void Song::print() const {
  if (!isPopulated) return;
  std::cout
  << "Index: " << index << '\n'
#ifdef IANDEBUG
  << "normalized:\n"
  << "\tTitle: " << getTitle() << '\n'
  << "\tArtist: " << getArtist() << '\n'
  << "\tDuration: " << getMinutes() << ':' << getSeconds() << '\n'
  << "\tAlbum: " << getAlbum() << '\n'
#endif
  << "Title: " << title << '\n'
  << "Arist: " << artist << '\n'
  << "Duration: " << minutes << ':'
  << std::setfill('0') << std::setw(2)
  << seconds << '\n'
  << "Album: " << album << "\n\n";
}

bool SongDB::print(const int index) const {
  if (!songs[index].isPopulated) return false;
  // Due to some restructuring, this isn't really needed, but I don't really
  // care to restructure the code. This has some nice features about it anyway.
  songs[index].print();
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
  
  int count = 0;
  char norm_query[MAX_STRING_SIZE] = {};
  char *token;
  char *cmpmethod; // used for time search query
  unsigned int minutes = 0;
  unsigned int seconds = 0;
  strncpy(norm_query, query, strlen(query));
  if (!(mode == TIME))
    normalize(norm_query);
  
  switch (mode) {
    case TITLE:
      for (int i = 0; i <= items; i++) {
        if (strstr(songs[i].getTitle(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break;
      
    case ARTIST:
      for (int i = 0; i <= items; i++) {
        if (strstr(songs[i].getArtist(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break;
      
    case ALBUM:
      for (int i = 0; i <= items; i++) {
        if (strstr(songs[i].getAlbum(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break;
      
    case TIME:
      // search for time. this will be fun!
      token = strtok(norm_query, " ");
      cmpmethod = token;
      // using a tokenizer, get each actual piece of information.
      token = strtok(nullptr, ":");
      minutes = atoi(token);
      token = strtok(nullptr, "\0");
      seconds = atoi(token);
      for (int i = 0; i <= items; i++) {
        switch (cmpmethod[0]) {
          case '<':
            if (strlen(cmpmethod) == 2) {
              if (cmpmethod[1] == '=') { // <=
                if (songs[i].minutes < minutes ||
                    (songs[i].minutes == minutes &&
                     songs[i].seconds <= seconds)) {
                      results[count++] = songs[i];
                    }
              } else {
                std::cerr << "Didn't understand comparison operator: "
                << cmpmethod << '\n';
              }
            } else { // <
              if (songs[i].minutes < minutes ||
                  (songs[i].minutes == minutes &&
                   songs[i].seconds < seconds)) {
                    results[count++] = songs[i];
                  }
            }
            break;
            
          case '=': // =
            if (songs[i].minutes == minutes &&
                songs[i].seconds == seconds) {
              results[count++] = songs[i];
            }
            break;
            
          case '>':
            if (strlen(cmpmethod) == 2) {
              if (cmpmethod[1] == '=') { // >=
                if (songs[i].minutes > minutes ||
                    (songs[i].minutes == minutes &&
                     songs[i].seconds >= seconds)) {
                      results[count++] = songs[i];
                    }
              } else {
                std::cerr << "Didn't understand comparison operator: "
                << cmpmethod << '\n';
              }
            } else { // >
              if (songs[i].minutes > minutes ||
                  (songs[i].minutes == minutes &&
                   songs[i].seconds > seconds)) {
                    results[count++] = songs[i];
                  }
            }
            break;
        }
      }
      break;
      
    default:
      for (int i = 0; i <= items; i++) {
        // if query
        if (strstr(songs[i].getTitle(), norm_query) != nullptr) {
          results[count++] = songs[i];
        } else if (strstr(songs[i].getArtist(), norm_query) != nullptr) {
          results[count++] = songs[i];
        } else if (strstr(songs[i].getAlbum(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break;
  }
  return count > 0;
}

SongDB::~SongDB() {
  std::ofstream f("songs.txt");
  for (auto s : songs) {
    // quit on first unpopulated entry.
    if (!s.isPopulated) break;
    f << s.title << ';'
    << s.artist << ';'
    << s.minutes << ';'
    << std::setfill('0') << std::setw(2)
    << s.seconds << ';'
    << s.album << '\n';
  }
  f.close();
  
  std::cout << "\nThanks for using Ian's music manager!\n";
}

Song& Song::setTitle(const char *val) {
  strncpy(title, val, MAX_STRING_SIZE);
  strncpy(_title, val, MAX_STRING_SIZE);
  normalize(_title);
  return *this;
}

Song& Song::setArtist(const char *val) {
  strncpy(artist, val, MAX_STRING_SIZE);
  strncpy(_artist, val, MAX_STRING_SIZE);
  normalize(_artist);
  return *this;
}

Song& Song::setMinutes(const unsigned int val) {
  minutes = val;
  _minutes = val;
  return *this;
}

Song& Song::setSeconds(const unsigned int val) {
  seconds = val;
  _seconds = val;
  return *this;
}

Song& Song::setAlbum(const char *val) {
  strncpy(album, val, MAX_STRING_SIZE);
  strncpy(_album, val, MAX_STRING_SIZE);
  normalize(_album);
  return *this;
}

Mode strToMode(const char* mode) {
  char lower[MAX_STRING_SIZE];
  strncpy(lower, mode, MAX_STRING_SIZE);
  normalize(lower);
  if (strncmp("title", lower, strlen(lower)) == 0) return TITLE;
  if (strncmp("artist", lower, strlen(lower)) == 0) return ARTIST;
  if (strncmp("time", lower, strlen(lower)) == 0) return TIME;
  if (strncmp("album", lower, strlen(lower)) == 0) return ALBUM;
  return OTHER; // This is necessary for the compiler with how the above series
  // of returns are structured, but it should never be reached.
}
