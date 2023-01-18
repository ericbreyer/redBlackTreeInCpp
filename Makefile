CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17
PROG  = ./bin/main.exe
OBJS = ./IMap/IMap.o ./linkedHashTable/linkedHashTable.o ./probeHashTable/probeHashTable.o ./rbtree/rbtree.o main.o

all: $(PROG)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(PROG): $(OBJS)
	$(CXX) -o $(PROG) $^ $(CXXFLAGS)

clean:
	rm -f *.o */*.o

# .PHONY: all clean

# FLAGS = -Wall -Werror -Wextra -std=c++17
# CC = g++

# all: main

# main: IMap.o lht.o pht.o rbt.o main.o
# 	$(CC) $(FLAGS) -g -o ./build/main.exe IMap.o lht.o pht.o rbt.o main.o
# main.o: main.cpp
# 	$(CC) $(FLAGS) -g -c main.cpp -o main.o 
# IMap.o: ./IMap/IMap.h
# 	$(CC) $(FLAGS) -g -c ./IMap/IMap.cpp -o IMap.o 
# lht.o: ./linkedHashTable/linkedHashTable.cpp
# 	$(CC) $(FLAGS) -g -c ./linkedHashTable/linkedHashTable.cpp -o lht.o 
# pht.o: ./probeHashTable/probeHashTable.cpp
# 	$(CC) $(FLAGS) -g -c ./probeHashTable/probeHashTable.cpp -o pht.o
# rbt.o: ./rbtree/rbtree.cpp
# 	$(CC) $(FLAGS) -g -c ./rbtree/rbtree.cpp -o rbt.o 

# clean:
# 	rm */*.o *.o