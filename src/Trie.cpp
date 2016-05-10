#include "Trie.h"

Trie::Trie()
{
    mRootNode = std::unique_ptr<TrieNode>(new TrieNode("", false));
}

bool Trie::insert(const std::string& word)
{
    if (word == "" || isValidWordInDictionary(word))
    {
        // Do nothing
        return;
    }

    recursiveInsertion(mRootNode.get(), word);
}

void Trie::recursiveInsertion(TrieNode* parent, const std::string& word)
{
    if (parent->word == word)
    {
        parent->isValidWord = true;
        return;
    }

    size_t parentLength = parent->word.size();
    std::string prefix = word.substr(0, parentLength+1);

    if (parent->childrenWords.find(prefix) == parent->childrenWords.end())
    {
        std::unique_ptr<TrieNode> newNode = std::unique_ptr(new TrieNode(prefix, false));
        parent->childrenWords.insert(std::move(newNode));
    }

    auto node = parent->childrenWords.find(prefix);

    recursiveInsertion(node->get(), word);
}

TrieNode* Trie::findNode(TrieNode* parent, const std::string& word)
{
    if (parent->word == word)
    {
        return parent;
    }
    else if (parent->word.length() == word.length())
    {
        return NULL;
    }

    size_t parentLength = parent->word.size();
    std::string prefix = word.substr(0, parentLength+1);

    auto nextNode = parent->childrenWords.find(prefix);

    if (nextNode == parent->childrenWords.end())
    {
        return NULL;
    }
    else
    {
        return findNode(nextNode->get(), 
    }
}
