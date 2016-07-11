CPPFLAGS = -W -Wextra -Wall --std=c++11

SRC=$(wildcard *.cpp)

OBJ=$(SRC:.cpp=.o)

rrp: $(OBJ)
	g++ -o $@ $^

.PHONY: clean

clean:
	rm $(OBJ)
