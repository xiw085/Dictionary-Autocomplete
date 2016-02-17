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
  TrieNode* node = root;
  std::string prevText = "";
  bool end = false;
  unsigned int freqs = 0;

  for(unsigned int i = 0 ; i < word.size(); ++i)
  {
  	if(i+1 == word.size())
	{
	  end = true;
	  freqs = freq;
	}

  	std::unordered_map<char, TrieNode*>::const_iterator find =
	  node->children.find(word[i]);
	
	if(find == node->children.end())
	{
      prevText = node->text;
	  prevText += word[i];

	  TrieNode* newNode = new TrieNode(end, prevText, freqs);
	  node->children.insert({word[i], newNode});
	  if(end)
	  {
	  	changeTreeFreq(word, freqs);
	  	return true;
	  }
	}
	if(!end)
	{
	  node = node->children[word[i]];
	}
	else if(!node->children[word[i]]->isWord)
	{
	  node = node->children[word[i]];
	  node->isWord = true;
	  node->freq = freqs;
	  changeTreeFreq(word, freqs);
	  return true;
	}
  }
  return false;
}

void DictionaryTrie::changeTreeFreq(std::string word, unsigned int freq)
{
  TrieNode* node = root;
  for(unsigned int i = 0; i < word.size(); ++i)
  {
  	if(freq > node->maxFreq)
	{
  	  node->maxFreq = freq;
	}
	if(freq < node->minFreq)
	{
	  node->minFreq = freq;
	}
	node = node->children[word[i]];
  }
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
	TrieNode* node = root;
	std::unordered_map<char,TrieNode*>::const_iterator find;

	for(unsigned int i = 0; i < word.size(); ++i)
	{
		find = node->children.find(word[i]);
		if(find == node->children.end())
		{
			return false;
		}
		node = node->children[word[i]];
	}
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
  std::vector<std::string> words(num_completions);

  TrieNode* node = root;
  std::unordered_map<char,TrieNode*>::const_iterator find;
  std::priority_queue<TrieNode*, std::vector<TrieNode*>, 
  					 TrieTreeComp> treePq;
  std::priority_queue<TrieNode*, std::vector<TrieNode*>,
  					 TrieNodeComp> nodePq;

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
  
  treePq.push(node);
  
  while(!treePq.empty())
  {
  	node = treePq.top();
	treePq.pop();
	if(node->isWord)
	{
	  nodePq.push(node);
	}
	
	find = node->children.begin();
	while(find != node->children.end())
	{
      treePq.push(find->second);
	  ++find;
	}
  }
	
  if(nodePq.size() < num_completions)
  {
  	words = std::vector<std::string>(nodePq.size());
  }

  for(unsigned int i = 0; i < num_completions && !nodePq.empty(); ++i)
  {
  	words[i] = nodePq.top()->text;
	nodePq.pop();
  }

  return words;
}
/*
void recursiveFind(auto nodePq, TrieNode* node)
{
  while(node->maxFreq != node->freq)
  {
  	nodePq.push( 
  }
}
*/
/* Destructor */
DictionaryTrie::~DictionaryTrie()
{
  if(root)
  {
  	deleteAll(root);
    delete root;
  }
  root = 0;
}

void DictionaryTrie::deleteAll(TrieNode* node)
{
  std::unordered_map<char,TrieNode*>::iterator find =
  	node->children.begin();

  while(find != node->children.end())
  {    
    deleteAll(find->second);
	delete find->second;
	++find;
  }
}

DictionaryTrie::TrieNode::TrieNode(bool isWord, std::string text
								, unsigned int freq)
{
  this->children = std::unordered_map<char, TrieNode*>();
  this->isWord = isWord;
  this->text = text;
  this->freq = freq;
  this->maxFreq = 0;
  this->minFreq = 0;
}

DictionaryTrie::TrieNode::~TrieNode()
{}

bool DictionaryTrie::TrieNode::maxLess(const TrieNode& other)
{
  return maxFreq < other.maxFreq;
}

bool DictionaryTrie::TrieNode::freqLess(const TrieNode& other)
{
  return freq < other.freq;
}

