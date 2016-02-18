#include "util.hpp"
#include "DictionaryTrie.hpp"
#include "DictionaryBST.hpp"
#include "DictionaryHashtable.hpp"
#include <string>
#include <fstream>

int main(int argc, char* argv[])
{
  if(argc < 5)
  {
  	std::cout << "Incorrect number of arguments." << std::endl;
	std::cout << "Usage: ./benchdict min_size step_size " << 
				 "num_iteration dictfile" << std::endl;
	return -1;
  }
  
  int min_size = std::atoi(argv[1]);
  int step_size = std::atoi(argv[2]);
  int num_iteration = std::atoi(argv[3]);
  std::string dictfile = argv[4];
  Timer T;
  long long time_duration;

  std::ifstream in;
  in.open(dictfile, std::ios::binary);
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

  std::cout << std::endl;
  std::cout << "DictionaryTrie" << std::endl;
  
  for(int i = 0; i < num_iteration; ++i)
  {
	if(len < min_size + i*step_size)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< min_size + i*step_size << std::endl;
	}
	
	DictionaryTrie* dictionary_trie = new DictionaryTrie();
	Utils::load_dict(*dictionary_trie, in, min_size + i*step_size);
	dictionary_trie->find("fjkdjfkdjfdkjfd");

	T.begin_timer();
	for(int j = 0; j < 10; ++j)
	{
	  dictionary_trie->find("fjkdjfkdjfdkjfd");
	}
	time_duration = T.end_timer() / 10;
	std::cout << min_size + i*step_size << "       " << time_duration 
			  << std::endl;

	delete dictionary_trie;	  
  }
  
  std::cout << std::endl;
  std::cout << "DictionaryBST" << std::endl;

  for(int i = 0; i < num_iteration; ++i)
  {
	if(len < min_size + i*step_size)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< min_size + i*step_size << std::endl;
	}

  	DictionaryBST* dictionary_BST = new DictionaryBST();
	Utils::load_dict(*dictionary_BST, in, min_size + i*step_size);
	dictionary_BST->find("fjkdjfkdjfdkjfd");

	T.begin_timer();
	for(int j = 0; j < 10; ++j)
	{
	  dictionary_BST->find("fjkdjfkdjfdkjfd");
	}
	time_duration = T.end_timer() / 10;
	std::cout << min_size + i*step_size << "       " << time_duration 
			  << std::endl;

	delete dictionary_BST;
  }

  std::cout << std::endl;
  std::cout << "DictionaryHashtable" << std::endl;

  for(int i = 0; i < num_iteration; ++i)
  {
	if(len < min_size + i*step_size)
	{
	  std::cout << "WARNING!! FILE HAS FEWER WORDS THAN: " 
	   			<< min_size + i*step_size << std::endl;
	}

  	DictionaryHashtable* dictionary_hashtable = new DictionaryHashtable();
	Utils::load_dict(*dictionary_hashtable, in, min_size + i*step_size);
	dictionary_hashtable->find("fjkdjfkdjfdkjfd");

	T.begin_timer();
	for(int j = 0; j < 10; ++j)
	{
	  dictionary_hashtable->find("fjkdjfkdjfdkjfd");
	}
	time_duration = T.end_timer() / 10;
	std::cout << min_size + i*step_size << "       " << time_duration 
			  << std::endl;
  
  	delete dictionary_hashtable;
  }
  
  in.close();
  return 0; 
}
