/*
 * Jae Song, jys023, A12042160
 * CSE 100 PA 3
 * 2/18/2016
 * FileName: DictionaryTrie.cpp
 * Description: this file will correctly implement the 
 * 				DictionaryTrie class which is essentially a 
 * 				multi-way trie dictionary
 *
 */

#include "util.hpp"
#include "DictionaryTrie.hpp"

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie()
{root = new TrieNode(false, "", 0);}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
  //declare variables
  TrieNode* node = root;
  std::string prevText = "";
  bool end = false;
  unsigned int freqs = 0;

  //loop through the chars in a word
  for(unsigned int i = 0 ; i < word.size(); ++i)
  {
    //special case for the last one
  	if(i+1 == word.size())
	{
	  //it's a word and get the correct freqs
	  end = true;
	  freqs = freq;
	}
    
	//iterator to the correct char in a map
  	std::unordered_map<char, TrieNode*>::const_iterator find =
	  node->children.find(word[i]);
	
	//if the key(char) doesn't exist...
	if(find == node->children.end())
	{
	  //first get the text...
      prevText = node->text;
	  prevText += word[i];
	  
	  //make a new TrieNode with correct arguments
	  TrieNode* newNode = new TrieNode(end, prevText, freqs);

	  //insert it to the correct place
	  node->children.insert({word[i], newNode});

	  //if it's the last char, then we need to change the maxFreq
	  if(end)
	  {
	  	changeTreeFreq(word, freqs);
	  	return true;
	  }
	}

	//continue to iterate through if it's not at end...
	if(!end)
	{
	  node = node->children[word[i]];
	}

	//if it's at end and it's not a word yet...
	else if(!node->children[word[i]]->isWord)
	{
	  //get the node and reset its member variables
	  node = node->children[word[i]];
	  node->isWord = true;
	  node->freq = freqs;
	  changeTreeFreq(word, freqs);
	  return true;
	}
  }
  
  //the word already exists? then return false
  return false;
}


/*
 * Name: changeTreeFreq
 * Parameter:string word, unsigned int freq
 * Return:void
 * Description: this method will essentially change the maxFreq of the
 * 				nodes if need be
 *
 */
void DictionaryTrie::changeTreeFreq(std::string word, unsigned int freq)
{
  //start from the root
  TrieNode* node = root;

  //iterate to the word
  for(unsigned int i = 0; i < word.size(); ++i)
  {  	
	//if freq is > than maxFreq change maxFreq
  	if(freq > node->maxFreq)
	{
  	  node->maxFreq = freq;
	}

    //continue to iterate through
	node = node->children[word[i]];
  }
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
  //start with root
  TrieNode* node = root;

  //iterator to find the correct one
  std::unordered_map<char,TrieNode*>::const_iterator find;

  //loop through to get to the word....
  for(unsigned int i = 0; i < word.size(); ++i)
  {
    //continue to iterate through
    find = node->children.find(word[i]);

	//if at anytime, the char needed doesn't exist...
	if(find == node->children.end())
	{
	  return false;
	}

	//otherwise move down the trie
	node = node->children[word[i]];
  }

  //return whether or not it's a word
  return node->isWord;
}

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
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, unsigned int num_completions)
{
  //final vector to return
  std::vector<std::string> words(num_completions);
  TrieNode* node = root;

  //iterator to go through the trie
  std::unordered_map<char,TrieNode*>::const_iterator find;

  //3 priority queues...
  //first one pops the one with the highest maxFreq
  std::priority_queue<TrieNode*, std::vector<TrieNode*>, 
  					 TrieTreeComp> treePq;

  //second one pops the one with the highest freq
  std::priority_queue<TrieNode*, std::vector<TrieNode*>,
  					 TrieNodeComp> nodePq;

  //third one pops the one with the lowest freq
  std::priority_queue<TrieNode*, std::vector<TrieNode*>,
  					 TrieNodeOppComp> nodeLessPq;

  //set the arbitrary size to be first num_completions
  unsigned int arbSize = num_completions;

  //first get to the correct node
  for(unsigned int i = 0; i < prefix.size(); ++i)
  {
  	find = node->children.find(prefix[i]);
	if(find == node->children.end())
	{
	  words = std::vector<std::string>(0);
	  return words;
	}
	node = node->children[prefix[i]];
  }
  
  //if the node is a word, insert it and increment arbSize
  if(node->isWord)
  {
    //we push to both nodePqs...
  	nodePq.push(node);
	nodeLessPq.push(node);
	++arbSize;
  }
  
  //now push to treePq
  treePq.push(node);
  unsigned int freqs = 0;
  bool end = false;
  bool found = false;

  while(!treePq.empty() && !end)
  {
    //if freqs is 0
  	if(!freqs)
	{
	  //reset the freqs and found flag
	  freqs = treePq.top()->maxFreq;
	  found = false;

	  //if it's still 0 then we can break
	  if(!freqs)
	  {
	  	break;
	  }
	}
	
	//we pop out the node with the highest maxFreq
  	node = treePq.top();
	treePq.pop();
	
	//we check through its children
	find = node->children.begin();
	while(find != node->children.end())
	{
	  
	  //if we find the node withe freq as maxFreq
	  if(find->second->freq == freqs)
	  {
	    //we found the biggest available...
		//we push to both nodePq and nodeLess because we want to
		//keep track of the smallest in the pq;
	  	nodePq.push(find->second);
		nodeLessPq.push(find->second);
	    
		//break if nodePq size has reached arbSize
		if(nodePq.size() == arbSize)
		{
		  //set end
		  end = true;
		  break;
		}

		//set found
		found = true;
	  }
	  
	  //if find->second is a word and
	  //its freq is greater than the freq of the smallest one
	  //already inserted or if nodePq is not "full" yet...
	  else if(find->second->freq &&
	  		  (find->second->freq > nodeLessPq.top()->freq
	  		  || nodePq.size() < num_completions))
	  {
	    
		//we push it.. this essentially pushes all the words that 
		//are explored while finding "maxFreq"
	  	nodePq.push(find->second);
		nodeLessPq.push(find->second);
		++arbSize;
	  }
	  
	  //we push everything to treePq to find maxFreq
      treePq.push(find->second);
	  ++find;
	}

	//once we found it, we can reset the freqs
	if(found)
	{
	  freqs = 0;
	}
  }
	
  //now we've got all the needed words... check size
  if(nodePq.size() < num_completions)
  {
    //if there's not enough reset size
  	words = std::vector<std::string>(nodePq.size());
  }
  
  //insert words according to size
  for(unsigned int i = 0; i < num_completions && !nodePq.empty(); ++i)
  {
  	words[i] = nodePq.top()->text;
	nodePq.pop();
  }
  
  //finally we return the vector
  return words;
}

/* Destructor */
DictionaryTrie::~DictionaryTrie()
{
  if(root)
  {
    //post-traversal of deleting all nodes
  	deleteAll(root);
    delete root;
  }
  //reset root
  root = 0;
}

/*
 * Name:deleteAll
 * Parameter: TrieNode* node
 * Return: void
 * Description: this will delete all nodes that are allocated on 
 * 				the heap
 *
 */
void DictionaryTrie::deleteAll(TrieNode* node)
{
  //iterator
  std::unordered_map<char,TrieNode*>::iterator find =
  	node->children.begin();

  //loop through its children
  while(find != node->children.end())
  {    
    //recursive call to delete its children...
    deleteAll(find->second);

	//finally delete itself
	delete find->second;
	++find;
  }
}

/*
 * Name:TrieNode constructor
 * Description: essential constructor for Trienode
 * Parameter: bool isWord, string text, unsigned int freq
 * Return: new TrieNode object
 */
DictionaryTrie::TrieNode::TrieNode(bool isWord, std::string text
								, unsigned int freq)
{
  //make children
  this->children = std::unordered_map<char, TrieNode*>();
  this->isWord = isWord;
  this->text = text;
  this->freq = freq;

  //maxFreq is automatically set to 0
  this->maxFreq = 0;
}

/* deconstructor for TrieNode */
DictionaryTrie::TrieNode::~TrieNode()
{}

/*
 * Name: maxLess
 * Parameter: TrieNode Other
 * Return: bool
 * Description: comparator used by TrieTreeComp
 */
bool DictionaryTrie::TrieNode::maxLess(const TrieNode& other)
{
  //if maxFreq are equal
  if(maxFreq == other.maxFreq)
  {
    //prioritize the word with smaller word
  	return text > other.text;
  }

  //otherwise pop out the one with highest maxFreq
  return maxFreq < other.maxFreq;
}

/*
 * Name: freqLess
 * Parameter: TrieNode other
 * Return: bool
 * Description: comparator used by TrieNodeComp
 */
bool DictionaryTrie::TrieNode::freqLess(const TrieNode& other)
{
  //if freqs are equal
  if(freq == other.freq)
  {
    //prioritize the word with smaller word
  	return text > other.text;	
  }

  //otherwise pop out the one with highest freq
  return freq < other.freq;
}

/*
 * Name: freqMore
 * Parameter: TrieNode other
 * Return: bool
 * Description: comparator used by TrieNodeOppComp
 */
bool DictionaryTrie::TrieNode::freqMore(const TrieNode& other)
{
  //again, if freqs are equal
  if(freq == other.freq)
  {
    //prioritize the word with smaller word
  	return text > other.text;
  }

  //otherwise pop out the one with smalelst freq
  return freq > other.freq;
}

