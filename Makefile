CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -pedantic

cTrieTest: CTrieTest.o CTrie.o
	$(CC) $(CFLAGS) -o cTrieTest CTrieTest.o CTrie.o -lm

wordHunt: wordHuntMain.o CTrie.o WordHunt.o
	$(CC) $(CFLAGS) -o wordHunt wordHuntMain.o CTrie.o WordHunt.o -lm

tTrieTest: TTrieTest.o
	$(CC) $(CFLAGS) -o tTrieTest TTrieTest.o -lm

TTrieTest.o: TTrieTest.cpp TTrie.inc TTrie.h
	$(CC) $(CFLAGS) -c TTrieTest.cpp

wordHuntMain.o: wordHuntMain.cpp CTrie.cpp CTrie.h WordHunt.h
	$(CC) $(CFLAGS) -c wordHuntMain.cpp CTrie.cpp

WordHunt.o: WordHunt.cpp WordHunt.h CTrie.cpp CTrie.h
	$(CC) $(CFLAGS) -c WordHunt.cpp CTrie.cpp

CTrieTest.o: CTrieTest.cpp CTrie.cpp CTrie.h
	$(CC) $(CFLAGS) -c CTrieTest.cpp CTrie.cpp

CTrie.o: CTrie.cpp CTrie.h
	$(CC) $(CFLAGS) -c CTrie.cpp

all: wordHunt cTrieTest tTrieTest

clean:
	rm -f *.o cTrieTest wordHunt tTrieTest
