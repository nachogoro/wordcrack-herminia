#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>
#include <set>
#include <memory>

class TrieNode
{
    public:
        const std::string word;
        bool isValidWord;
        std::set<std::shared_ptr<TrieNode> > childrenWords;

        TrieNode(const std::string& val, bool isWord)
            : word(val),
              isValidWord(isWord)
        { }
};

class Trie {
    public:
        Trie();
        void insert(const std::string& word);
        bool isValidWordInDictionary(const std::string& word) const;
        bool isPrefixOfOtherWords(const std::string& prefix) const;

    private:
        void recursiveInsertion(TrieNode* parent, const std::string& word);
        TrieNode* findNode(TrieNode* parent, const std::string& word) const;

    private:
        // The dictionary will be shared by several threads, each taking a copy
        // of it. In order for the object to be copyable, we cannot have
        // unique_ptr
        std::shared_ptr<TrieNode> mRootNode;
};

#endif
