#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>
#include <vector>
#include <memory>

class TrieNode
{
    public:
        const std::string word;
        bool isValidWord;
        std::vector<std::unique_ptr<TrieNode> > childrenWords;

        TrieNode(const std::string& val, bool isWord);
};

class Trie {
    public:
        Trie();
        void insert(const std::string& word);
        bool isValidWordInDictionary(const std::string& word) const;
        bool isPrefixOfOtherWords(const std::string& prefix) const;

    private:
        void recursiveInsertion(TrieNode* parent, const std::string& word);
        TrieNode* recursiveFindNode(TrieNode* parent, const std::string& word) const;

    private:
        std::unique_ptr<TrieNode> mRootNode;
};

#endif
