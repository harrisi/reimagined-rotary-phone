#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>
#include "libian.hpp"
#include "Songs.hpp"

char doMenu();
void add(SongDB&);
void remove(SongDB&);
void search(const SongDB&);
void view(const SongDB&);

// May want to move elsewhere and abstract away some of the details.
void loadFile(SongDB&, const char = ';');

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
  
  // I think I'll be able to abstract away the reading of information from
  // loadFile to some function ("songFromStream" or the like) which reads in a
  // stream of delimiter separated values from a stringstream and adds a song
  // object to the db passed to it.
  std::cout << "Song title: ";
  getString(buf);
  song_db.songs[song_db.items].title(buf);
  //std::strncpy(song_db.songs[song_db.items].title(), buf, MAX_STRING_SIZE);
  
  std::cout << "Song artist: ";
  getString(buf);
  song_db.songs[song_db.items].artist(buf);
  //std::strncpy(, buf, MAX_STRING_SIZE);
  
  std::cout << "Song minutes: ";
  song_db.songs[song_db.items].minutes(getInt(0));
  
  std::cout << "Song seconds: ";
  song_db.songs[song_db.items].seconds(getInt(0, 59));
  
  std::cout << "Song album: ";
  getString(buf);
  song_db.songs[song_db.items].album(buf);
  //std::strncpy(, buf, MAX_STRING_SIZE);
  
  song_db.songs[song_db.items].isPopulated(true);
  
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
  while ((input = getInt()) < 0 || input > song_db.items) {
    std::cout << "Index out of range! Try again: ";
  }
  // could check if equal here for some extra cool maybe efficiency!
  while (++input < song_db.items) {
    song_db.songs[input - 1].title(song_db.songs[input].title());
    //std::strcpy(, );
    song_db.songs[input - 1].artist(song_db.songs[input].artist());
    //std::strcpy(, );
    song_db.songs[input - 1].minutes(song_db.songs[input].minutes());
    song_db.songs[input - 1].seconds(song_db.songs[input].seconds());
    song_db.songs[input - 1].album(song_db.songs[input].album());
    //std::strcpy(, );
  }
  song_db.songs[--song_db.items].isPopulated(false);
  //song_db.items--;
}

void doCommand(const char command) {
  switch (command) {
    case '?':
      std::cout << "helpful message!\n";
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
  char query[MAX_STRING_SIZE] {0}; // holds actual query. This will exclude
                                   // specific "category" to search for.
  // Decide how to search (assignment requires artist and album)
  //std::cout << "Search for artist or album? ";
  
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
      if ((lookahead = getchar()) == EOF || lookahead == '\n') stop = true;
      //startOf = true; // this is in the wrong place.
    }
    switch (buf) {
      case ':':
        if (startOf) {
          doCommand(lookahead);
        } else {
          mode = strToMode(query);
          // clear query
          std::memset(query, '\0', sizeof query);
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

void loadFile(SongDB& song_db, const char delim) {
  enum Items {
    TITLE,
    ARTIST,
    MINUTES,
    SECONDS,
    ALBUM
  } fsm = TITLE;
  char in[MAX_STRING_SIZE];
  std::ifstream
  f("/Users/ian/misc/school/cs162/projects/project2/project2/songs.txt");
  //Items fsm = TITLE;
  //int i = 0; // this overwrites from the beginning every time.
  //f.open;
  
  if (!f.is_open()) {
    std::cerr << "Failed to load file.\n";
    // XXX: Define error numbering properly.
    exit(1);
  }
  
  // get until delim
  // This needs to check for the case of a newline. How to do this will be
  // potentially tricky.
  while (f.get(in, MAX_STRING_SIZE, delim) ||
         song_db.items <= MAX_SONG_DB_SIZE) {
    if (f.eof()) break; // got to EOF, break.
    if(f.peek() == delim) {
      f.get(); // consume delimiter
    } else { // next char isn't delim, panic (this shouldn't happen)
      std::cerr << "Error reading in data from file; invalid format!\n";
      exit(1); // XXX: Define error numbering properly.
    }
    // assign tokens to values by column
    // title;artist;mm;ss;album
    switch (fsm) {
      case TITLE:
        song_db.songs[song_db.items].title(in);
        //std::strncpy(, in, MAX_STRING_SIZE);
        fsm = ARTIST;
        break;
        
      case ARTIST:
        song_db.songs[song_db.items].artist(in);
        //std::strncpy(, in, MAX_STRING_SIZE);
        fsm = MINUTES;
        break;
        
      case MINUTES:
        song_db.songs[song_db.items].minutes(static_cast<unsigned int>(atoi(in)));
        fsm = SECONDS;
        break;
        
      case SECONDS:
        song_db.songs[song_db.items].seconds(static_cast<unsigned int>(atoi(in)));
        fsm = ALBUM;
        break;
        
      case ALBUM:
        song_db.songs[song_db.items].album(in);
        //std::strncpy(, in, MAX_STRING_SIZE);
        fsm = TITLE;
        song_db.songs[song_db.items].isPopulated(true);
        // XXX: I hate this.
        if (f.peek() == '\n') { // using peek and panicking is reasonable.
          f.get(); // consume newline.
          song_db.items++; // this is the actual tracking of the index which seems silly.
        } else {
          if (f.peek() == EOF) { // this catches the case where the file doesn't
                                 // contain a newline before EOF.
            f.get();
            break;
          }
          std::cerr << "Error in parser! Expected newline.\n";
          exit(1); // XXX: Define error numbering properly.
        }
        break;
        
      default:
        std::cerr << "Error in FSM! Aborting.\n";
        // XXX: Define error numbering properly.
        exit(1);
        break;
    }
  }
  // entire file should be loaded into memory now
  
  // close file
  f.close();
}
