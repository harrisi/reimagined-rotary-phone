#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "libian.hpp"
#include "Songs.hpp"

char doMenu();
void add(SongDB&);
void remove(SongDB&);
void search(const SongDB&);
void view(const SongDB&);

// May want to move elsewhere and abstract away some of the details.
void loadFile(SongDB&, const char* = ";");

int main() {
  char input;
  std::cout << "Welcome to Ian's music library program!\n";
  SongDB songs;
  loadFile(songs);
  while ((input = doMenu()) != 'q') {
    switch (tolower(input)) {
      case 'a':
        add(songs);
        break;
        
      case 'r':
        remove(songs);
        break;
        
      case 's':
        search(songs);
        break;
        
      case 'v':
        view(songs);
        break;
        
      default:
        std::cout << "Invalid input! Try again:\n";
        break;
    }
  }
  
  return 0;
}

char doMenu() {
  char res = 'q';
  
  std::cout << "Options:\n"
  << "(A)dd a song.\n"
  << "(R)emove a song.\n"
  << "(S)earch for a song.\n"
  << "(V)iew all songs.\n"
  << "(Q)uit.\n"
  << "Selection> ";
  
  while (!(std::cin >> res)) {
    if (std::cin.eof()) break; // hacky(?) way of dealing with Ctrl-D, although
                               // only when no input preceeds Ctrl-D..
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input! Try again: ";
  }
  
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  
  return res;
}

void add(SongDB& song_db) {
  char buf[MAX_STRING_SIZE];
  char *minutes = nullptr;
  char *seconds = nullptr;
  
  // I think I'll be able to abstract away the reading of information from
  // loadFile to some function ("songFromStream" or the like) which reads in a
  // stream of delimiter separated values from a stringstream and adds a song
  // object to the db passed to it.
  std::cout << "Song title: ";
  getString(buf);
  song_db.songs[song_db.items].setTitle(buf);
  //std::strncpy(song_db.songs[song_db.items].title, buf, MAX_STRING_SIZE);
  std::cout << "Song artist: ";
  
  getString(buf);
  song_db.songs[song_db.items].setArtist(buf);
  //std::strncpy(song_db.songs[song_db.items].artist, buf, MAX_STRING_SIZE);
  
  // Neat. But it doesn't actually have any code for maintaining useful times
  // (i.e., 0 <= seconds < 60). Oh well. This is at least cool.
  std::cout << "Song duration (e.g. '4:20'): ";
  getString(buf);
  minutes = strtok(buf, ":");
  if (minutes != nullptr)
    seconds = strtok(nullptr, "\n");
  while (minutes == nullptr ||
         seconds == nullptr ||
         !isInt(minutes) ||
         !isInt(seconds) ||
         atoi(seconds) < 0 ||
         atoi(seconds) > 59) {
    std::cout << "Invalid input! Try again: ";
    getString(buf);
    minutes = strtok(buf, ":");
    if (minutes != nullptr)
      seconds = strtok(nullptr, "\n");
    
  }
  
  song_db.songs[song_db.items].setMinutes(atoi(minutes));
  song_db.songs[song_db.items].setSeconds(atoi(seconds));
  
  std::cout << "Song album: ";
  getString(buf);
  song_db.songs[song_db.items].setAlbum(buf);
  
  song_db.songs[song_db.items].isPopulated = true;
  song_db.songs[song_db.items].index = song_db.items;
  
  // Make sure we maintain index correctness.
  song_db.items++;
}

// This should probably be a member function and then have another function
// doRemove or something which would deal with all the specifics of prompting
// or whatever. The member function would just take the index to be removed and
// deal with all the organization of the db.
void remove(SongDB& song_db) {
  int input;
  std::cout
  << "Which song would you like to remove? (Numeric index, i.e., 42): ";
  while ((input = getInt()) < 0 || input >= song_db.items) {
    std::cout << "Index out of range! Try again: ";
  } // I don't like this because it forces the user to delete an item. If they
    // accidentally get here, they are forced to delete something or force exit
    // the program.
  // could check if equal here for some extra cool maybe efficiency!
  while (++input < song_db.items) {
    // As a consequence of the changes, I can now chain these methods for some
    // neat looking calls. Cool!
    song_db.songs[input - 1].setTitle(song_db.songs[input].title)
      .setArtist(song_db.songs[input].artist)
      .setMinutes(song_db.songs[input].minutes)
      .setSeconds(song_db.songs[input].seconds)
      .setAlbum(song_db.songs[input].album);
  }
  song_db.songs[--song_db.items].isPopulated = false;
}

void doCommand(const char command) {
  // This could also include manual functionality options such as reading in
  // from the song DB, saving to the song DB, etc., as well as general
  // ":help add" sort of syntax that would give more specific details for each
  // menu option.
  switch (command) {
    case '?':
      std::cout << "This is the help message for searching.\n"
      << "Searching is currently not fully implemented, but the basic\n"
      << "structure of a search is [field]: query\n"
      << "where field is an optional field selector of:\n"
      << "\ttitle\n"
      << "\tartist\n"
      << "\ttime\n"
      << "\talbum\n"
      << "Using one of these fields as the start of the query allows for more\n"
      <<
      "specific search functionality. If the field selector is omitted, the\n"
      <<
      "default behavior is to search for every field for each item. This is\n"
      << "slower, but is still useful.\n";
      break;
      
    default:
      std::cout << "Command not found: " << command << '\n';
      break;
  }
}

// Should also be a member function, I think.
void search(const SongDB& song_db) { // should return something.
  // I don't know if it should return an array of songs or a specific search
  // object. A search object seems kind of nice. It would contain an array of
  // Song objects and just be populated with search results. Handling indexing
  // is a bit tricky, though.. For useful search purposes, it needs to show the
  // index for the SongDB passed. Bleh.
  Mode mode = OTHER;
  char buf;
  char lookahead = -1;
  bool startOf = true;
  bool stop = false;
  bool modeset = false;
  Song results[100]; // Arbitrary number. Memory allocation is a beach.
  char query[MAX_STRING_SIZE] = {}; // Holds actual query. This will exclude
                                    // specific "category" to search for.
  
  // For some reason if I don't do this, the results are "cached," but I'm not
  // sure why. This is also favorable to `Song results[n] = {};`, because this
  // approach, for some reason, uses a crazy amount of memory. Memory
  // allocation is a real issue with my current approach.
  for (int i = 0; ; i++) {
    if (results[i].isPopulated) {
      results[i].isPopulated = false;
    } else {
      break;
    }
  }
  
  // By default, search all fields (artist, album, title). This would require
  // an exact match, which I don't like. At the very least I will probably
  // lowercase. I could also tokenize on ':' and have the option to format query
  // like "artist: some artist" to search only for artist field.
  // I also would like to have the ability to search for song duration:
  // "time: > 4:20" would return all songs that have a duration greater than
  // 4:20. This would be rather difficult, but would be super cool, I think.
  
  std::cout << "Enter search query (or \":?\" for help): ";
  // read input one by one
  
  while (!stop) {
    if (lookahead != -1) {
      buf = lookahead;
      if ((lookahead = getchar()) == EOF ||
          lookahead == '\n') stop = true;
      startOf = false;
    } else { // should only happen on first loop.
      if ((buf = getchar()) == EOF || buf == '\n') break;
      stop = (lookahead = getchar()) == EOF || lookahead == '\n';
    }
    switch (buf) {
      case ':':
        if (startOf) {
          doCommand(lookahead);
        } else if (!modeset) {
          mode = strToMode(query);
          // clear query
          memset(query, '\0', sizeof query);
          modeset = true;
        } else {
          query[strlen(query)] = buf;
        }
        break;
        
      default:
        query[strlen(query)] = buf;
        break;
    }
  }
  query[strlen(query)] = '\0';
  // tokenize query ?
  
  // This is where the actual searching would happen. Perhaps this is where we
  // call song_db.search(query, mode = OTHER) which does.. something. The
  // results will be rather messy.
  
  if (song_db.search(query, results, mode)) { // this could use a SongDB, which
                                              // could be nice (?)
    for (int i = 0; i < MAX_SONG_DB_SIZE; i++) {
      // bail on first bad result
      if (!results[i].isPopulated) break;
      results[i].print();
    }
  }
  
#if IANDEBUG
  std::cout << "mode: ";
  switch (mode) {
    case TITLE: std::cout << "title\n";
      break;
    case ARTIST: std::cout << "artist\n";
      break;
    case TIME: std::cout << "time\n";
      break;
    case ALBUM: std::cout << "album\n";
      break;
    default: std::cout << "other\n";
      break;
  }
  std::cout << "query: " << query << '\n';
#endif
}

// This would be nice if I could have something similar to more/less, but that
// would be a lot of work.
void view(const SongDB& song_db) {
  // this could also just loop until i >= song_db.items..
  for (int i = 0; i < MAX_SONG_DB_SIZE; i++) {
    // To save cycles, as soon as Song.print() returns false (meaning the
    // current object's `isPopulated` field is false), break out of the loop and
    // stop all attempts at printing the song. This assumes there will never be
    // any breaks in the song db, which seems like a reasonable assumption (or
    // hope).
    if(!song_db.print(i))
      break;
  }
}

void loadFile(SongDB& song_db, const char *delim) {
  char in[MAX_STRING_SIZE];
  std::ifstream f("songs.txt");
  char *token;
  
  if (!f.is_open()) {
    std::cerr << "Failed to load file.\n";
    // XXX: Define error numbering properly.
    exit(1);
  }
  
  // Rewrite. Much nicer, although a bit flimsy.
  while (f.getline(in, std::numeric_limits<std::streamsize>::max()) &&
         song_db.items <= MAX_SONG_DB_SIZE) {
    if (f.eof()) break;
    token = strtok(in, delim);
    song_db.songs[song_db.items].setTitle(token);
    token = strtok(nullptr, delim);
    song_db.songs[song_db.items].setArtist(token);
    token = strtok(nullptr, delim);
    song_db.songs[song_db.items].setMinutes(atoi(token));
    token = strtok(nullptr, delim);
    song_db.songs[song_db.items].setSeconds(atoi(token));
    token = strtok(nullptr, delim);
    song_db.songs[song_db.items].setAlbum(token);
    song_db.songs[song_db.items].isPopulated = true;
    song_db.songs[song_db.items].index = song_db.items;
    song_db.items++;
  }
  // entire file should be loaded into memory now
  
  f.close();
}
