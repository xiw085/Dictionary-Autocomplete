#include "util.hpp"
#include "DictionaryBST.hpp"

/* Create a new Dictionary that uses a BST back end */
DictionaryBST::DictionaryBST()
{dictionary = std::set<std::string>();}

/* Insert a word into the dictionary. */
bool DictionaryBST::insert(std::string word)
{
	std::pair<std::set<std::string>::const_iterator,bool> pair =
		dictionary.insert(word);
	return pair.second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryBST::find(std::string word) const
{
	std::set<std::string>::const_iterator find = dictionary.find(word);
	if(find == dictionary.end())
	{
		return false;
	}
	return true;
}

/* Destructor */
DictionaryBST::~DictionaryBST(){}
