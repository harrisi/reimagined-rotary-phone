#include <iostream>
#include <cstring>
#include <limits>

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

int main() {
  char input;
  std::cout << "Welcome to Ian's music library program!\n";
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