/*
 * Jae Song, jys023, A12042160
 * CSE 100 PA 3
 * 2/18/2016
 * FileName: DictionaryHashtable.cpp
 * Description: this file will implement the class DictionaryHashtable
 *
 */

#include "util.hpp"
#include "DictionaryHashtable.hpp"

/* Create a new Dictionary that uses a Hashset back end */
DictionaryHashtable::DictionaryHashtable()
{dictionary = std::unordered_set<std::string>();}

/* Insert a word into the dictionary. */
bool DictionaryHashtable::insert(std::string word)
{
	std::pair<std::unordered_set<std::string>::const_iterator,bool> pair 
		= dictionary.insert(word);

	//return whether or not it was inserted correctly
	return pair.second;
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryHashtable::find(std::string word) const
{
	std::unordered_set<std::string>::const_iterator find = 
		dictionary.find(word);

	//check if it was found or not
	if(find == dictionary.end())
	{
		return false;
	}
	return true;
}

/* Destructor */
DictionaryHashtable::~DictionaryHashtable(){}
