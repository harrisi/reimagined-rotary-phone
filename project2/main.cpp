#include <iostream>
#include <sstream>
#include <cstring>
#include <limits>
#include <fstream>
#include <cstdlib>
#include "libian.hpp"
#include "Songs.hpp"

char doMenu();
void add(SongDB&);
void remove(SongDB&);
void search(const SongDB&, std::istream& = std::cin);
void view(const SongDB&);
void help();

// May want to move elsewhere and abstract away some of the details.
void loadFile(SongDB&, const char* = ";");

void doMenuEx(SongDB&);
void doCommand(const char);
void doCommandEx(const char*);

int main() {
  std::cout << "Welcome to Ian's music library program!\n";
  SongDB songs;
  loadFile(songs);
  doMenuEx(songs);
  return 0;
}

void help() {
  char *token = strtok(nullptr, "\t\n ");
  if (!token) {// help had no arguments"
    std::cout << "HELP\n"
    << "This is general information about the program and how to use it. For\n"
    << "information about a specific feature, try passing an argument to help\n"
    << "from the main menu. Valid arguments are:\n"
    << "\tADD:\tinformation about adding a song to the library.\n"
    << "\tREMOVE:\tinformation about removing a song from the library.\n"
    << "\tSEARCH:\tinformation about searching for a song in the library.\n"
    << "\tVIEW:\tinformation about viewing all songs in the library.\n"
    << "\tQUIT:\tinformation about quitting the program.\n";
    // Here I would like to include more general information about the program
    // such as some clarifications of what a menu, submenu, field, query, etc.
    // are. Also it could detail the actual logical program flow of the program.
    return;
  }
  if (strncmp(token, "add", maxlen(token, "add")) == 0) {
    std::cout << "HELP ADD\n"
    << "Adding a song is a straightforward sequential process. To add a song,\n"
    << "enter 'add' at the main menu when prompted for entry. You will then\n"
    << "guided through a series of steps to enter aspects of the song, which\n"
    << "are the song title, artist, duration, and album.\n";
  } else if (strncmp(token, "remove", maxlen(token, "remove")) == 0) {
    std::cout << "HELP REMOVE\n"
    << "Removing a song can be done by typing 'remove' at the main menu when\n"
    << "prompted for entry. You will then be asked for the index of the song\n"
    << "to be removed. If you are cannot remember the index of the song you\n"
    << "wish to delete, you can alternatively give the remove option a search\n"
    << "query, which can return a selection of songs and will return you to\n"
    << "the removal operation, which allows you to again enter the index.\n";
  } else if (strncmp(token, "search", maxlen(token, "search")) == 0) {
    std::cout << "HELP SEARCH\n"
    << "Searching for a song can be difficult in large libraries. To make the\n"
    << "process easier, there are a number of options available for\n"
    << "searching. Search results can be narrowed by specifying the 'field'\n"
    << "you are interested in, e.g., 'artist: pink floyd' will return all of\n"
    << "the songs in your library with an artist matching 'pink floyd'. For\n"
    << "more information about searching, the search submenu gives more\n"
    << "specific information.\n";
  } else if (strncmp(token, "view", maxlen(token, "view")) == 0) {
    std::cout << "HELP VIEW\n"
    << "The view option will just give a dump of all songs in your library.\n"
    << "This functionality is not as useful as searching as the library grows\n"
    << "but can be useful if you just want a complete overview of the library\n"
    << "for whatever reason. This can also be accomplished by searching for\n"
    << "an empty string, i.e., type nothing before pressing enter in the\n"
    << "search submenu.\n";
  } else if (strncmp(token, "quit", maxlen(token, "quit")) == 0) {
    std::cout << "HELP QUIT\n"
    << "Quitting the program will write the in-memory music library to disk\n"
    << "in the file used to initially read the music library on startup. Note\n"
    << "that this will overwrite any modifications to the file that were made\n"
    << "to the music file while this program was running. This behavior is\n"
    << "expected to change in a future release.\n";
  } else {
    std::cout << "Couldn't understand help message option.\n";
  }
}

void doCommandEx(const char *tok) {
  if (strncmp(tok, "help", strlen("help")) == 0) {
    help();
  } else if (strncmp(tok, "?", strlen("?")) == 0) {
    std::cout << "got '?'\n";
  }
}

void doMenuEx(SongDB& songs) {
  char in[MAX_STRING_SIZE], *token;
  bool quit = false;
  do {
    std::cout << "Options:\n"
    << "(Add) a song.\n"
    << "(Remove) a song.\n"
    << "(Search) for a song.\n"
    << "(View) all songs.\n"
    << "(Help)\n"
    << "(Quit).\n"
    << "Enter one of the above words in parentheses (parentheses not required)."
    << '\n';
    getString(in, "Selection> ");
    token = strtok(in, "\t\n ");
    normalize(token);
#ifdef IANDEBUG
    std::cout << "token: " << token << '\n';
#endif
    if (token[0] == ':') {
      doCommandEx(&token[1]);
    } else {
      if (strncmp(token, "add", strlen("add")) == 0 ||
          strncmp(token, "a", strlen("a")) == 0) {
        add(songs);
      } else if (strncmp(token, "remove", maxlen(token, "remove")) == 0 ||
                 strncmp(token, "r", strlen(token)) == 0) {
        remove(songs);
      } else if (strncmp(token, "search", maxlen(token, "search")) == 0 ||
                 strncmp(token, "s", strlen(token)) == 0) {
        search(songs);
      } else if (strncmp(token, "view", maxlen(token, "view")) == 0 ||
                 strncmp(token, "v", strlen(token)) == 0) {
        view(songs);
      } else if (strncmp(token, "help", maxlen(token, "help")) == 0 ||
                 strncmp(token, "h", strlen(token)) == 0) {
        help();
      } else if (strncmp(token, "quit", maxlen(token, "quit")) == 0 ||
                 strncmp(token, "q", strlen(token)) == 0) {
        quit = true;
      } else {
        std::cout << "Didn't understand '" << token << "'.\n";
        continue;
      }
    }
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  } while (!quit);
//  while ((token = strtok(nullptr, "\t\n "))) {
//    std::cout << "TOKEN: " << token << '\n';
//  }
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
    if (std::cin.eof()) break; // Hacky(?) way of dealing with Ctrl-D, although
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
  //std::cout << "Song title: ";
  getString(buf, "Song title: ");
  song_db.songs[song_db.items].setTitle(buf);
  //std::cout << "Song artist: ";
  
  getString(buf, "Song artist: ");
  song_db.songs[song_db.items].setArtist(buf);
  
  //std::cout << "Song duration (e.g. '4:20'): ";
  getString(buf, "Song duration (e.g. '4:20'): ");
  minutes = strtok(buf, ":");
  if (minutes != nullptr)
    seconds = strtok(nullptr, "\n");
  // Repeat..
  while (minutes == nullptr || // ..if minutes is null..
         seconds == nullptr || // ..or seconds is null..
         !isInt(minutes) || // ..or minutes isn't an int..
         !isInt(seconds) || // ..or seconds isn't an int..
         atoi(seconds) < 0 || // ..or seconds is less than 0..
         atoi(seconds) > 59) { // ..or seconds is greater than 59.
    //std::cout << "Invalid input! Try again: ";
    getString(buf);
    minutes = strtok(buf, ":");
    if (minutes != nullptr)
      seconds = strtok(nullptr, "\n");
    
  }
  
  song_db.songs[song_db.items].setMinutes(atoi(minutes));
  song_db.songs[song_db.items].setSeconds(atoi(seconds));
  
  //std::cout << "Song album: ";
  getString(buf, "Song album: ");
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
  char *token;
  token = strtok(nullptr, "\t\n ");
  if (token) {
    if (isInt(token)) {
      input = atoi(token);
      if (input < 0 || input >= song_db.items) {
        std::cerr << "Index out of range!\n";
        return;
      }
    } else { // "remove someQuery"
      std::stringstream teststream;
      do {
        normalize(token);
        teststream << token << ' ';
      } while ((token = strtok(nullptr, "\t\n ")));
      search(song_db, teststream);
      goto lol;
    }
  } else {
  lol:
    std::cout
    << "Which song would you like to remove? (Numeric index, i.e., 42): ";
    while ((input = getInt()) < 0 || input >= song_db.items) {
      std::cout << "Index out of range! Try again: ";
    } // I don't like this because it forces the user to delete an item. If they
    // accidentally get here, they are forced to delete something or force exit
    // the program.
  }
  // Could check if equal here for some extra cool maybe efficiency!
  while (++input < song_db.items) {
    // As a consequence of the changes, I can now chain these methods for some
    // neat looking calls. Cool!
    song_db.songs[input - 1].setTitle(song_db.songs[input].getTitle())
      .setArtist(song_db.songs[input].getArtist())
      .setMinutes(song_db.songs[input].getMinutes())
      .setSeconds(song_db.songs[input].getSeconds())
      .setAlbum(song_db.songs[input].getAlbum());
  }
  song_db.songs[--song_db.items].isPopulated = false;
}

void doCommand(const char command) {
  char buf[MAX_STRING_SIZE];
  
  // This could also include manual functionality options such as reading in
  // from the song DB, saving to the song DB, etc., as well as general
  // ":help add" sort of syntax that would give more specific details for each
  // menu option.
  switch (command) {
    case '?':
      std::cout << "This is the help message for searching.\n"
      << "Searching follows the basic format of:\n"
      << "\t[FIELD]: QUERY\n"
      << "Where field is an optional field selector of:\n"
      << "\ttitle\n"
      << "\tartist\n"
      << "\ttime\n"
      << "\talbum\n"
      << "And QUERY is the search query.\n"
      << "Using one of these fields as the start of the query allows for more\n"
      <<
      "specific search functionality. If the field selector is omitted, the\n"
      <<
      "default behavior is to search for every field for each item. This is\n"
      << "slower, but is still useful.\n"
      << "For specific help with a FIELD, try :help FIELD\n";
      break;
      
    case 'h':
      while (!isspace(getchar()))
        ; // consume optional "elp"
      getString(buf);
      normalize(buf);
      if (strstr(buf, "title") != nullptr) {
        std::cout << "Title help text.\n"
        << "Search for title by prefixing the query with 'title:'. This will\n"
        << "limit the search results to only those whose title contains the\n"
        << "query.\n"
        << "\nEXAMPLE:\n"
        << "\tSelection> s\n"
        << "\tEnter search query (or \":?\" for help): title: us and them\n"
        << "\tIndex: 42\n"
        << "\tTitle: Us and Them\n"
        << "\tArist: Pink Floyd\n"
        << "\tDuration: 7:49\n"
        << "\tAlbum: The Dark Side of the Moon\n"
        << "\nNOTES:\n"
        << "\tCapitalization and spaces are ignored.\n";
      } else if (strstr(buf, "artist") != nullptr) {
        std::cout << "Artist help text.\n"
        <<
        "Search for artist by prefixing the query with 'artist:'. This will\n"
        << "limit the search results to only those whose artist contains the\n"
        << "query.\n"
        << "\nEXAMPLE:\n"
        << "\tSelection> s\n"
        << "\tEnter search query (or \":?\" for help): artist: pink floyd\n"
        << "\tIndex: 42\n"
        << "\tTitle: Us and Them\n"
        << "\tArist: Pink Floyd\n"
        << "\tDuration: 7:49\n"
        << "\tAlbum: The Dark Side of the Moon\n"
        << "\nNOTES:\n"
        << "\tCapitalization and spaces are ignored.\n";
      } else if (strstr(buf, "time") != nullptr) {
        std::cout << "Time help text.\n"
        << "Search by time by prefixing the query with 'time:'. This will\n"
        << "limit the search results to only those satisfying the predicate\n"
        << "passed as the query.\n"
        << "\nEXAMPLE:\n"
        << "\tSelection> s\n"
        << "\tEnter search query (or \":?\" for help): time: > 7:00\n"
        << "\tIndex: 42\n"
        << "\tTitle: Us and Them\n"
        << "\tArist: Pink Floyd\n"
        << "\tDuration: 7:49\n"
        << "\tAlbum: The Dark Side of the Moon\n"
        << "\nNOTES:\n"
        << "\tThe format of this is a bit picky. Whitespace _does_ matter,\n"
        << "\tto an extent. Try to separate the comparison operator from the\n"
        << "\ttime being used, i.e., `time: <= 3:30` and not `time: <=3:30`.\n"
        << "\tThe valid comparison operators are:\n"
        << "\t\t<\t(song time is LESS THAN given time)\n"
        << "\t\t<=\t(song time is LESS THAN OR EQUAL TO given time)\n"
        << "\t\t>\t(song time is GREATER THAN given time)\n"
        << "\t\t>=\t(song time is GREATER THAN OR EQUAL TO given time)\n"
        << "\t\t=\t(song time is EQUAL TO given time)\n"
        << "\t\t==\t(song time is EQUAL TO given time)\n"
        << "\t\t!\t(song time is NOT EQUAL TO given time)\n"
        << "\t\t!=\t(song time is NOT EQUAL TO given time)\n";
      } else if (strstr(buf, "album") != nullptr) {
        std::cout << "Album help text.\n"
        << "Search for album by prefixing the query with 'album:'. This will\n"
        << "limit the search results to only those whose album contains the\n"
        << "query.\n"
        << "\nEXAMPLE:\n"
        << "\tSelection> s\n"
        << "\tEnter search query (or \":?\" for help): album: dark side\n"
        << "\tIndex: 42\n"
        << "\tTitle: Us and Them\n"
        << "\tArist: Pink Floyd\n"
        << "\tDuration: 7:49\n"
        << "\tAlbum: The Dark Side of the Moon\n"
        << "\nNOTES:\n"
        << "\tCapitalization and spaces are ignored.\n";
      } else {
        std::cout << "General help text.\n"
        << "Use :help <option> where <option> is one of the following:\n"
        << "\ttitle\n"
        << "\tartist\n"
        << "\ttime\n"
        << "\talbum\n";
      }
      break;
      
    default:
      std::cerr << "Command not found: " << command << '\n';
      break;
  }
}

// Should also be a member function, I think.
void search(const SongDB& song_db, std::istream& stream) { // should return something (?)
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
  Song results[MAX_RESULT_SIZE]; // Memory allocation is a beach.
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
  
  // The above was successfully implemented with great success!
  
  std::cout << "Enter search query (or \":?\" for help): ";
  
  // Read input one by one. Store lookahead and use for EOL and such.
  while (!stop) {
    if (lookahead != -1) { // If lookahead has usable value, use it.
      buf = lookahead;
      if ((lookahead = stream.get()) == EOF || // This fills lookahead again.
          lookahead == '\n') stop = true;
      startOf = false;
    } else { // Should only happen on first loop.
      if ((buf = stream.get()) == EOF || buf == '\n') break; // Fill buf.
      // Fill lookahead.
      stop = (lookahead = stream.get()) == EOF || lookahead == '\n';
    }
    switch (buf) {
      case ':':
        if (startOf) { // If ':' is the first character, it's a command. Use
                       // lookahead as the command.
          doCommand(lookahead);
          stream.clear();
          stream.ignore();
          return;
          // This feels very weird, but the purpose of this is so that the user
          // can use ':' after the mode. Time searching was giving me problems.
          // In short, this allows for "time: > 4:20" without clobbering "time"
          // as the mode.
        } else if (!modeset) { // If mode hasn't been set, query is mode.
          mode = strToMode(query); // Set mode to Mode value based on query.
          memset(query, '\0', sizeof query); // Clear query.
          modeset = true; // Mode has been set, will only happen once.
        } else { // This only happens when mode has already been set, and we're
                 // not at the start of the buffer. Allows multiple :'s in a
                 // query.
          // Non-command, non-mode-setter, just add buf to query.
          query[strlen(query)] = buf;
        }
        break;
        
      default: // Just part of query.
        query[strlen(query)] = buf;
        break;
    }
  }
  query[strlen(query)] = '\0';
  
  // This is where the actual searching would happen. Perhaps this is where we
  // call song_db.search(query, mode = OTHER) which does.. something. The
  // results will be rather messy.
  
  // Above was implemented successfully!
  
  if (song_db.search(query, results, mode)) { // This could use a SongDB, which
                                              // could be nice (?).
    for (int i = 0; i < MAX_RESULT_SIZE; i++) {
      // Bail on first bad result.
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
  // This could also just loop until i >= song_db.items..
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
  while (f.getline(in, std::numeric_limits<std::streamsize>::max())) {
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
  // Entire file should be loaded into memory now.
  
  f.close();
}
