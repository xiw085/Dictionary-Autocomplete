/*
 * Name: Jae Song, A12042160, jys023
 * CSE 100 PA 3
 * Date: 2/18/2016
 * FileName: benchdict.cpp
 * Description: this file checks the runtime of the three
 * 				distinct dictionary classes
 *
 */
#include "util.hpp"
#include "DictionaryTrie.hpp"
#include "DictionaryBST.hpp"
#include "DictionaryHashtable.hpp"
#include <string>
#include <fstream>

//main method
int main(int argc, char* argv[])
{
  //make sure you have the correct number of arguments...
  if(argc < 5)
  {
  	std::cout << "Incorrect number of arguments." << std::endl;
	std::cout << "Usage: ./benchdict min_size step_size " << 
				 "num_iteration dictfile" << std::endl;
	return -1;
  }
  
  //get the variables needed
  int min_size = std::atoi(argv[1]);
  int step_size = std::atoi(argv[2]);
  int num_iteration = std::atoi(argv[3]);
  std::string dictfile = argv[4];
  Timer T;
  long long time_duration;
  
  //open up the file...
  std::ifstream in;
  in.open(dictfile, std::ios::binary);
  
  //check if input is valid
  if(!in.is_open())
  {
  	std::cout << "Invalid input file" << std::endl;
	return -1;
  }
  
  //get the length
  int len = 0;
  std::string text = "";

  while(1)
  {
  	getline(in, text);

	if(in.eof())
	{
	  break;
	}
	++len;
  }

  //start with DictionaryTrie
  std::cout << std::endl;
  std::cout << "DictionaryTrie" << std::endl;
  
  //loop through the num_iterations
  for(int i = 0; i < num_iteration; ++i)
  {
    int count = min_size + i*step_size;
    //check if number of words is enough
	if(len < count)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< count << std::endl;
	}
	
	//make the dictionaryTrie
	DictionaryTrie* dictionary_trie = new DictionaryTrie();
	Utils::load_dict(*dictionary_trie, in, count);

	//don't time first time for cache...
	dictionary_trie->find("vnkdnvknd");

	//begin timer...
	T.begin_timer();
	for(int j = 0; j < 100; ++j)
	{
	  //find it 10 times
	  for(int h = 0; h < 10; ++h)
	  {
	  	dictionary_trie->find("vnkdnvknd");
	  }
	}

	//average it
	time_duration = T.end_timer() / 100;

	//print it out
	std::cout << count << "\t"<< time_duration << std::endl;

	//delete the Trie
	delete dictionary_trie;	  
  }
  
  //move on to DictionaryBST
  std::cout << std::endl;
  std::cout << "DictionaryBST" << std::endl;

  //we basically do the same thing as DictionaryTrie but it's for
  //DictionaryBST...
  for(int i = 0; i < num_iteration; ++i)
  {
    int count = min_size + i*step_size;
	if(len < count)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< count << std::endl;
	}

	//instead of DictionaryTrie we make bst
  	DictionaryBST* dictionary_BST = new DictionaryBST();
	Utils::load_dict(*dictionary_BST, in, count);

	//we throw away the first time for cache...
	dictionary_BST->find("vnkdnvknd");

	T.begin_timer();
	for(int j = 0; j < 100; ++j)
	{
	  for(int h = 0; h < 10; ++h)
	  {
	  	dictionary_BST->find("vnkdnvknd");
	  }
	}
	time_duration = T.end_timer() / 100;
	std::cout << count << "\t" << time_duration << std::endl;

	delete dictionary_BST;
  }

  //finally DictionaryHashtable
  std::cout << std::endl;
  std::cout << "DictionaryHashtable" << std::endl;

  //same thing as the two above examples, but for DictionaryHashtable
  for(int i = 0; i < num_iteration; ++i)
  {
    int count = min_size + i*step_size;
	if(len < count)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< count << std::endl;
	}

  	DictionaryHashtable* dictionary_hashtable = new DictionaryHashtable();
	Utils::load_dict(*dictionary_hashtable, in, count);

	//throw away first time for cache...
	dictionary_hashtable->find("vnkdnvknd");

	T.begin_timer();
	for(int j = 0; j < 100; ++j)
	{
	  for(int h = 0; h < 10; ++h)
	  {
	  	dictionary_hashtable->find("vnkdnvknd");
	  }
	}
	time_duration = T.end_timer() / 100;
	std::cout << count << "\t" << time_duration << std::endl;
  
  	delete dictionary_hashtable;
  }
  
  //close file
  in.close();
  return 0; 
}
