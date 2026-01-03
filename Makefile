# BASIC FLAGS
#CPPFLAGS = -std=c++14 -g -O3

# ENABLE FOR DJB2 hash function (better distribution, faster queries)
#CPPFLAGS = -std=c++14 -g -O3 -DUSE_DJB2_HASH

# PRODUCTION BUILD with all warnings and DJB2 hash enabled
CPPFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -DUSE_DJB2_HASH

all: main
clean: 
	rm -rf *.o *.out 
cleanwin:
	del *.o
	del *.out
main: main.o parser.o database_index.o
	g++ $(CPPFLAGS) main.o parser.o database_index.o -o a.out

main.o: main.cpp parser.h avl_tree.h linked_list.h database_index.h database.h
	g++ $(CPPFLAGS) -c main.cpp

parser.o: parser.cpp parser.h
	g++ $(CPPFLAGS) -c parser.cpp
database_index.o: database_index.cpp database_index.h
	g++ $(CPPFLAGS) -c database_index.cpp
