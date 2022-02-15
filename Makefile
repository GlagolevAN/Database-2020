#CPPFLAGS = -std=c++14 -g -O3

#ENABLE FOR nicer hash function
#CPPFLAGS = -std=c++14 -g -O3 -DZACHET #-Wall -Wextra -pedantic -Werror -ffast-math -g -pg -O3 -DZACHET


#DO NOT ACTIVATE(There are a lot of flags below)
CPPFLAGS = -std=c++14 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -DZACHET #-Wno-suggest-attribute=format

all: main1
clean: 
	rm -rf *.o *.out 
cleanwin:
	del *.o
	del *.out
main1: main1.o razbor.o database1.o
	g++ $(CPPFLAGS) main1.o razbor.o database1.o -o a.out

main1.o: main1.cpp razbor.h tree.h list.h database1.h database2.h
	g++ $(CPPFLAGS) -c main1.cpp

razbor.o: razbor.cpp razbor.h
	g++ $(CPPFLAGS) -c razbor.cpp
database1.o: database1.cpp database1.h
	g++ $(CPPFLAGS) -c database1.cpp
