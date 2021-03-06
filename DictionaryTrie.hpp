/**
 *  CSE 100 PA3 C++ Autocomplete
 *  Authors: Jor-el Briones, Christine Alvarado
 */

#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>

/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 *
 */
class DictionaryTrie
{
public:

  /* Create a new Dictionary that uses a Trie back end */
  DictionaryTrie();

  /* Insert a word with its frequency into the dictionary.
   * Return true if the word was inserted, and false if it
   * was not (i.e. it was already in the dictionary or it was
   * invalid (empty string) */
  bool insert(std::string word, unsigned int freq);

  /* Return true if word is in the dictionary, and false otherwise */
  bool find(std::string word) const;

  /* Return up to num_completions of the most frequent completions
   * of the prefix, such that the completions are words in the dictionary.
   * These completions should be listed from most frequent to least.
   * If there are fewer than num_completions legal completions, this
   * function returns a vector with as many completions as possible.
   * If no completions exist, then the function returns a vector of size 0.
   * The prefix itself might be included in the returned words if the prefix
   * is a word (and is among the num_completions most frequent completions
   * of the prefix)
   */
  std::vector<std::string>
  predictCompletions(std::string prefix, unsigned int num_completions);

  /* Destructor */
  ~DictionaryTrie();

private:
  // Add your own data members and methods here
  
  /*
   * This class will implement the TrieNode class which is basically
   * a class for each node in the multi-way trie
   *
   */
  class TrieNode
  {
  	public:
	  
	  //member variables
 	  bool isWord;
	  std::unordered_map<char,TrieNode*> children;
	  std::string text;
	  unsigned int freq;

	  //maxFreq is the maxfreq of a node's subtree
	  unsigned int maxFreq;
		
	  //constructor
	  TrieNode(bool isWord, std::string text, unsigned int freq);
		
	  //deconstructor
	  ~TrieNode();
		
	  //comparison used for TrieTreeComp
	  bool maxLess(const TrieNode& other);

	  //comparison used for TrieNodeComp
	  bool freqLess(const TrieNode& other);

      //comparison used for TrieNodeOppComp	  
	  bool freqMore(const TrieNode& other);
  };
  
  /*
   * Comparison class used for priority queue that pops out
   * the node with the highest maxFreq first
   */
  class TrieTreeComp 
  {
  	public:
	  bool operator()(TrieNode*& lhs, TrieNode*& rhs) const 
	  {
	    return (*lhs).maxLess(*rhs);
	  }
  };

  /*
   * Comparison class used for priority queue that pops out
   * the node with the highest freq first
   *
   */
  class TrieNodeComp 
  {
  	public:
	  bool operator()(TrieNode*& lhs, TrieNode*& rhs) const
	  {
	    return (*lhs).freqLess(*rhs); 
	  }
  };

  /*
   * Comparison class used for priority queue that pops out
   * the node with the lowest freq first
   *
   */
  class TrieNodeOppComp 
  {
	public:
	  bool operator()(TrieNode*& lhs, TrieNode*& rhs) const
	  {
	    return (*lhs).freqMore(*rhs);
	  }
  };
  
  //root of the multi-way trie
  TrieNode* root;
  
  //function that changes the maxfreq is the nodes
  void changeTreeFreq(std::string word, unsigned int freq);
  
  //helper function that correctly implements the deconstructor
  void deleteAll(TrieNode* node);
};

#endif // DICTIONARY_TRIE_HPP
