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
        std::set<std::unique_ptr<TrieNode> > childrenWords;

        TrieNode(const std::string& val, bool isWord)
        {
            word = val;
            isValidWord = isWord;
        }

        ~TrieNode()
        {
            // Delete all the nodes below this one.
            for (auto child = childrenWords.begin(); child != childrenWords.end(); ++child)
            {
                delete *child;
            }
        }
};

class Trie {
    public:
        Trie();

        bool isValidWordInDictionary(const std::string& word) const;

        bool isPrefixOfOtherWords(const std::string& prefix) const;

        bool insert(const std::string& word);

    private:
        void recursiveInsertion(TrieNode* parent, const std::string& word);
        void nodeExists(const std::string& word);
        TrieNode* findNode(TrieNode* parent, const std::string& word);

    private:
        std::unique_ptr<TrieNode> mRootNode;
};

#endif
