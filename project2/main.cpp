#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>
#include "libian.hpp"

// maximum amount of songs possible. I might try increasing this to 50k to mimic
// Google Play Music's capacity.
const int MAX_SONG_DB_SIZE = 2000;

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
  bool print() const;
  bool isPopulated;
};

struct SongDB {
  int items;
  Song songs[MAX_SONG_DB_SIZE];
  void insert(Song);
  void search();
};

bool Song::print() const {
  if (!isPopulated) return false;
  std::cout << "Title: " << this->title << '\n'
  << "Arist: " << this->artist << '\n'
  << "Duration: " << this->minutes << ':' << this->seconds << '\n'
  << "Album: " << this->album << "\n\n";
  return true;
}

char doMenu();
void add(SongDB&);
void remove(SongDB&);
void search(const SongDB&);
void view(const SongDB&);
// not sure if I need an explicit `quit` function.
// after thought: I will probably use it to hold all the "cleanup" logic.
// additionally, ideally I think if this was in a destructor all the cleanup
// logic would be handled by the runtime for me, which would be nice.
void quit();

// May want to move elsewhere and abstract away some of the details.
void loadFile(SongDB&, const char = ';');

int main() {
  char input;
  std::cout << "Welcome to Ian's music library program!\n";
  SongDB songs;
  //Song songs[MAX_SONG_DB_SIZE];
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
  // here: input == 'q'
  quit(); // handles cleanup, which might be writing file to disk or applying
          // a patch from the diff currently living in ".$1.swp" file. This
          // will possibly be moved to an object destructor, since it allows
          // the runtime to deal with all this nonsense in weird situations I
          // don't want to plan for.
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

void getString(char *buf) {
  while(!std::cin.get(buf, MAX_STRING_SIZE - 1)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid input! Try again: ";
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInt() {
  int res;
  while (!(std::cin >> res)) {
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
  // Make sure we maintain index correctness.
  song_db.items++;
  
  // I think I'll be able to abstract away the reading of information from
  // loadFile to some function ("songFromStream" or the like) which reads in a
  // stream of delimeter separated values from a stringstream and adds a song
  // object to the db passed to it.
  std::cout << "Song title: ";
  getString(buf);
  std::strncpy(song_db.songs[song_db.items].title, buf, MAX_STRING_SIZE);
  
  std::cout << "Song artist: ";
  getString(buf);
  std::strncpy(song_db.songs[song_db.items].artist, buf, MAX_STRING_SIZE);
  
  std::cout << "Song minutes: ";
  song_db.songs[song_db.items].minutes = getInt();
  
  std::cout << "Song seconds: ";
  song_db.songs[song_db.items].seconds = getInt();
  
  std::cout << "Song album: ";
  getString(buf);
  std::strncpy(song_db.songs[song_db.items].album, buf, MAX_STRING_SIZE);
  
  song_db.songs[song_db.items].isPopulated = true;
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
  while (++input <= song_db.items) {
    std::strcpy(song_db.songs[input - 1].title, song_db.songs[input].title);
    std::strcpy(song_db.songs[input - 1].artist, song_db.songs[input].artist);
    song_db.songs[input - 1].minutes = song_db.songs[input].minutes;
    song_db.songs[input - 1].seconds = song_db.songs[input].seconds;
    std::strcpy(song_db.songs[input - 1].album, song_db.songs[input].album);
  }
  song_db.songs[song_db.items].isPopulated = false;
  song_db.items--;
}

void search(const SongDB& song_db) {
  std::cout << "search\n";
}

// This would be nice if I could have something similar to more/less, but that
// would be a lot of work.
void view(const SongDB& song_db) {
  for (int i = 0; i < MAX_SONG_DB_SIZE; i++) {
    if (i > song_db.items) break;
      std::cout << "Index: " << i << '\n';
    // To save cycles, as soon as Song.print() returns false (meaning the
    // current object's `isPopulated` field is false), break out of the loop and
    // stop all attempts at printing the song. This assumes there will never be
    // any breaks in the song db, which seems like a reasonable assumption (or
    // hope).
    if(!song_db.songs[i].print())
      break;
    //std::cout << "Index: " << i << '\n'; // this is pretty ugly and hacky.
  }
}

void quit() {
  std::cout << "quit\n";
}

void loadFile(SongDB& song_db, const char delim) {
  enum Items {
    TITLE,
    ARTIST,
    MINUTES,
    SECONDS,
    ALBUM
  };
  char in[MAX_STRING_SIZE];
  std::ifstream f;
  Items fsm = TITLE;
  //int i = 0; // this overwrites from the beginning every time.
  f.open("songs.txt");
  
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
        std::strncpy(song_db.songs[song_db.items].title, in, MAX_STRING_SIZE);
        fsm = ARTIST;
        break;
        
      case ARTIST:
        std::strncpy(song_db.songs[song_db.items].artist, in, MAX_STRING_SIZE);
        fsm = MINUTES;
        break;
        
      case MINUTES:
        song_db.songs[song_db.items].minutes = static_cast<unsigned int>(atoi(in));
        fsm = SECONDS;
        break;
        
      case SECONDS:
        song_db.songs[song_db.items].seconds = static_cast<unsigned int>(atoi(in));
        fsm = ALBUM;
        break;
        
      case ALBUM:
        std::strncpy(song_db.songs[song_db.items].album, in, MAX_STRING_SIZE);
        fsm = TITLE;
        song_db.songs[song_db.items].isPopulated = true;
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
