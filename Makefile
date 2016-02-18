# A simple makefile for CSE 100 P3

CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=-g

all: benchtrie benchdict util.o

benchdict: util.o DictionaryTrie.o DictionaryBST.o DictionaryHashtable.o

benchtrie: util.o DictionaryTrie.o DictionaryBST.o DictionaryHashtable.o

DictionaryTrie.o: DictionaryTrie.hpp

DictionaryBST.o: DictionaryBST.hpp

DictionaryHashtable.o: DictionaryHashtable.hpp

util.o: util.hpp

clean:
	rm -f benchtrie benchdict *.o core* *~

