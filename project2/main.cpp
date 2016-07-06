#include <iostream>
#include <cstring>
#include <limits>
#include <fstream>

// I don't like the size restriction for this, but it will have to do. Unless I
// just read in forever until a delimiter is found. But that would get super
// K&R, which I don't know if I want to do.
#define MAX_SONG_STRING 1000
// maximum amount of songs possible. I might try increasing this to 50k to mimic
// Google Play Music's capacity.
#define MAX_SONG_DB_SIZE 2000

// private is best, but I don't want to deal with that right now.
struct Song {
  // I could model this object better and define a destructor that would do all
  // the cleanup for me for free. This would probably be my favorite approach,
  // but it would require remodelling the whole program flow, most likely. For
  // now I will be much more rudimentary.
  char title[MAX_SONG_STRING];
  char artist[MAX_SONG_STRING];
  unsigned int minutes; // Although it's highly unlikely, using an int instead
  // of a char allows for songs longer than 255 minutes.
  unsigned char seconds; // Seconds are forced into a range of 0-59, which means
  // the smallest type to contain it is an unsigned char.
  char album[MAX_SONG_STRING];
};

char doMenu();
void add();
void remove();
void search();
void view();
// not sure if I need an explicit `quit` function.
// after thought: I will probably use it to hold all the "cleanup" logic.
// additionally, ideally I think if this was in a destructor all the cleanup
// logic would be handled by the runtime for me, which would be nice.
void quit();

// May want to move elsewhere and abstract away some of the details.
void loadFile(Song[], const char = ';');

int main() {
  char input;
  std::cout << "Welcome to Ian's music library program!\n";
  Song songs[MAX_SONG_DB_SIZE];
  loadFile(songs);
  while ((input = doMenu()) != 'q') {
    switch (tolower(input)) {
      case 'a':
        add();
        break;
        
      case 'r':
        remove();
        break;
        
      case 's':
        search();
        break;
        
      case 'v':
        view();
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

void add() {
  std::cout << "add\n";
}

void remove() {
  std::cout << "remove\n";
}

void search() {
  std::cout << "search\n";
}

void view() {
  std::cout << "view\n";
}

void quit() {
  std::cout << "quit\n";
}

void loadFile(Song song_db[], const char delim) {
  enum Items {
    TITLE,
    ARTIST,
    MINUTES,
    SECONDS,
    ALBUM
  };
  char in[MAX_SONG_STRING];
  std::ifstream f;
  //Song song_db[MAX_SONG_DB_SIZE];
  Items fsm = Items::TITLE;
  int i = 0;
  char next = '\0';
  f.open("songs.txt");
  
  if (!f.is_open()) {
    std::cerr << "Failed to load file.\n";
    // XXX: Define error numbering properly.
    exit(1);
  }
  
  // get until delim
  // This needs to check for the case of a newline. How to do this will be
  // potentially tricky.
  while (//!(f.eof()) ||
         (next = f.peek()) == '\n' || // I don't think this will work as I want..
         f.get(in, MAX_SONG_STRING, delim) ||
         i >= MAX_SONG_DB_SIZE) {
    if (f.eof()) break;
    if (next == '\n') i++; // increment index.
    if(f.peek() == delim) {
      f.get(); // consume delimiter
    } else { // next char isn't delim, panic (this shouldn't happen)
      std::cerr << "Error reading in data from file; invalid format!\n";
      exit(1); // XXX: Define error numbering properly.
    }
    // assign tokens to values by column
    // title;artist;mm;ss;album
    switch (fsm) {
      case Items::TITLE:
        std::strncpy(song_db[i].title, in, MAX_SONG_STRING);
        fsm = Items::ARTIST;
        break;
        
      case Items::ARTIST:
        std::strncpy(song_db[i].artist, in, MAX_SONG_STRING);
        fsm = Items::MINUTES;
        break;
        
      case Items::MINUTES:
        song_db[i].minutes = static_cast<unsigned int>(atoi(in));
        fsm = Items::SECONDS;
        break;
        
      case Items::SECONDS:
        song_db[i].seconds = static_cast<unsigned char>(atoi(in));
        fsm = Items::ALBUM;
        break;
        
      case Items::ALBUM:
        std::strncpy(song_db[i].album, in, MAX_SONG_STRING);
        fsm = Items::TITLE;
        //i++; // this is the actual tracking of the index, which is super silly.
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