.PHONY: all

LIBS=-lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio #-lsfml-network

HEADERS=tilegrid.h

all: main

main: main.cpp $(HEADERS)
	clang++ $(LIBS) -std=c++17 -Wall -Wextra -Wpedantic -o main main.cpp

debug: main.cpp $(HEADERS)
	clang++ $(LIBS) -std=c++17 -g -Wall -Wextra -Wpedantic -o main main.cpp

run: main
	./main

clean:
	rm -rf main *.o *.dSYM
