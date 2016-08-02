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
  return title;
}

const char* Song::getNormTitle() const {
  return norm_title;
}

const char* Song::getArtist() const {
  return artist;
}

const char* Song::getNormArtist() const {
  return norm_artist;
}

unsigned int Song::getMinutes() const{
  return minutes;
}

unsigned int Song::getSeconds() const {
  return seconds;
}

const char* Song::getAlbum() const {
  return album;
}

const char* Song::getNormAlbum() const {
  return norm_album;
}

void Song::print() const {
  if (!isPopulated) return;
  std::cout
  << "Index: " << index << '\n'
#ifdef IANDEBUG
  << "normalized:\n"
  << "\tTitle: " << norm_title << '\n'
  << "\tArtist: " << norm_artist << '\n'
  << "\tDuration: " << minutes << ':' << seconds << '\n'
  << "\tAlbum: " << norm_album << '\n'
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
  
  // The above was implemented with reasonable success.
  
  // Either way, clearly the searching for the assignment specifications will be
  // rather barebones and not as nice as full-featured as I would like. Alas,
  // there are only so many hours in the day.
  
  int count = 0;
  char norm_query[MAX_STRING_SIZE] = {};
  char *token;
  char *cmpmethod; // Used for time search query.
  unsigned int minutes = 0;
  unsigned int seconds = 0;
  strncpy(norm_query, query, strlen(query));
  if (!(mode == TIME)) // Don't normalize for time queries.
    normalize(norm_query);
  
  switch (mode) {
    case TITLE:
      for (int i = 0; i <= items; i++) {
        if (count >= MAX_RESULT_SIZE) break;
        if (strstr(songs[i].getNormTitle(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break; // case TITLE
      
    case ARTIST:
      for (int i = 0; i <= items; i++) {
        if (count >= MAX_RESULT_SIZE) break;
        if (strstr(songs[i].getNormArtist(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break; // case ARTIST
      
    case ALBUM:
      for (int i = 0; i <= items; i++) {
        if (count >= MAX_RESULT_SIZE) break;
        if (strstr(songs[i].getNormAlbum(), norm_query) != nullptr) {
          results[count++] = songs[i];
        }
      }
      break; // case ALBUM
      
    // I would also like to allow for "time: 4:20" as a shortcut to
    // "time: = 4:20".
    case TIME:
      // search for time. this will be fun!
      token = strtok(norm_query, " ");
      if (token == nullptr) {
        std::cerr << "Error reading query: '"
        << norm_query
        << "'! Stopping search.\n";
        return false;
      }
      cmpmethod = token;
      // using a tokenizer, get each actual piece of information.
      token = strtok(nullptr, ":");
      if (token == nullptr) {
        std::cerr << "Error reading query: '"
        << norm_query
        << "'! Stopping search.\n";
        return false;
      }
      minutes = atoi(token);
      token = strtok(nullptr, "\0");
      if (token == nullptr) {
        std::cerr << "Error reading query: '"
        << norm_query
        << "'! Stopping search.\n";
        return false;
      }
      seconds = atoi(token);
      for (int i = 0; i <= items; i++) {
        switch (cmpmethod[0]) {
          case '<':
            if (strlen(cmpmethod) == 2) {
              if (cmpmethod[1] == '=') { // <=
                if (songs[i].getMinutes() < minutes ||
                    (songs[i].getMinutes() == minutes &&
                     songs[i].getSeconds() <= seconds)) {
                      if (count >= MAX_RESULT_SIZE) break;
                      results[count++] = songs[i];
                    }
              } else {
                std::cerr << "Didn't understand comparison operator: "
                << cmpmethod << '\n';
                return false;
              }
            } else { // <
              if (songs[i].getMinutes() < minutes ||
                  (songs[i].getMinutes() == minutes &&
                   songs[i].getSeconds() < seconds)) {
                    if (count >= MAX_RESULT_SIZE) break;
                    results[count++] = songs[i];
                  }
            }
            break; // case '<'
            
          case '=': // =
            if (strlen(cmpmethod) == 2 &&
                cmpmethod[1] != '=') {
              std::cerr << "Didn't understand comparison operator: "
              << cmpmethod << '\n';
              return false;
            }
            if (songs[i].getMinutes() == minutes &&
                songs[i].getSeconds() == seconds) {
              if (count >= MAX_RESULT_SIZE) break;
              results[count++] = songs[i];
            }
            break; // case '='
            
          case '>':
            if (strlen(cmpmethod) == 2) {
              if (cmpmethod[1] == '=') { // >=
                if (songs[i].getMinutes() > minutes ||
                    (songs[i].getMinutes() == minutes &&
                     songs[i].getSeconds() >= seconds)) {
                      if (count >= MAX_RESULT_SIZE) break;
                      results[count++] = songs[i];
                    }
              } else {
                std::cerr << "Didn't understand comparison operator: "
                << cmpmethod << '\n';
                return false;
              }
            } else { // >
              if (songs[i].getMinutes() > minutes ||
                  (songs[i].getMinutes() == minutes &&
                   songs[i].getMinutes() > seconds)) {
                    if (count >= MAX_RESULT_SIZE) break;
                    results[count++] = songs[i];
                  }
            }
            break; // case '>'
            
          case '!': // !
            if (strlen(cmpmethod) == 2) {
              if (cmpmethod[1] == '=') { // !=
                if (songs[i].getMinutes() != minutes ||
                    (songs[i].getMinutes() == minutes &&
                     songs[i].getSeconds() != seconds)) {
                      if (count >= MAX_RESULT_SIZE) break;
                      results[count++] = songs[i];
                    }
              } else {
                std::cerr << "Didn't understand comparison operator: "
                << cmpmethod << '\n';
                return false;
              }
            } else { // ! (note: I'm not sure if I like allowing this operator)
              if (songs[i].getMinutes() != minutes ||
                  (songs[i].getMinutes() == minutes &&
                   songs[i].getSeconds() != seconds)) {
                    if (count >= MAX_RESULT_SIZE) break;
                    results[count++] = songs[i];
                  }
            }
            break; // case '!'
        }
      }
      break; // case TIME
      
    default:
      for (int i = 0; i <= items; i++) {
        if (strstr(songs[i].getNormTitle(), norm_query) != nullptr) {
          if (count >= MAX_RESULT_SIZE) break;
          results[count++] = songs[i];
        } else if (strstr(songs[i].getNormArtist(), norm_query) != nullptr) {
          if (count >= MAX_RESULT_SIZE) break;
          results[count++] = songs[i];
        } else if (strstr(songs[i].getNormAlbum(), norm_query) != nullptr) {
          if (count >= MAX_RESULT_SIZE) break;
          results[count++] = songs[i];
        }
      }
      break; // default
  }
  if (++count >= MAX_RESULT_SIZE)
    std::cerr << "Max search results (" << MAX_RESULT_SIZE << ") returned!\n";
  return count > 0;
}

SongDB::~SongDB() {
  std::ofstream f("songs.txt");
  for (int i = 0; i < items; i++) {
    // quit on first unpopulated entry.
    if (!songs[i].isPopulated) break;
    f << songs[i].getTitle() << ';'
    << songs[i].getArtist() << ';'
    << songs[i].getMinutes() << ';'
    << std::setfill('0') << std::setw(2)
    << songs[i].getSeconds() << ';'
    << songs[i].getAlbum() << '\n';
  }
  f.close();
  free(songs);
  
  std::cout << "\nThanks for using Ian's music manager!\n";
}

SongDB::SongDB() {
  songs = new Song();
}

Song& Song::setTitle(const char *val) {
  title = strdup(val);
  norm_title = strdup(val);
  normalize(norm_title);
  return *this;
}

Song& Song::setArtist(const char *val) {
  artist = strdup(val);
  norm_artist = strdup(val);
  normalize(norm_artist);
  return *this;
}

Song& Song::setMinutes(const unsigned int val) {
  minutes = val;
  return *this;
}

Song& Song::setSeconds(const unsigned int val) {
  seconds = val;
  return *this;
}

Song& Song::setAlbum(const char *val) {
  album = strdup(val);
  norm_album = strdup(val);
  normalize(norm_album);
  return *this;
}

Song::~Song() {
  free(title);
  free(norm_title);
  free(artist);
  free(norm_artist);
  free(album);
  free(norm_album);
}

Mode strToMode(const char* mode) {
  char lower[MAX_STRING_SIZE];
  strncpy(lower, mode, MAX_STRING_SIZE);
  normalize(lower);
  if (strncmp("title", lower, strlen("title")) == 0) return TITLE;
  if (strncmp("artist", lower, strlen("artist")) == 0) return ARTIST;
  if (strncmp("time", lower, strlen("time")) == 0) return TIME;
  if (strncmp("album", lower, strlen("album")) == 0) return ALBUM;
  std::cerr << "Mode could not be read!\n";
  return OTHER;
}
