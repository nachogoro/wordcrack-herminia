#include "Trie.h"

#include <algorithm>

namespace
{
    std::vector<std::unique_ptr<TrieNode>>::iterator findNodeInSetStartingWith(
            std::vector<std::unique_ptr<TrieNode> >& nodes,
            const std::string& word)
    {
        return std::find_if(nodes.begin(), nodes.end(),
                [word](const std::unique_ptr<TrieNode>& x)
                {
                return x->word.substr(0, word.size()) == word;
                }
                );
    }
} // end of anonymous namespace

TrieNode::TrieNode(const std::string& val, bool isWord)
    : word(val),
      isValidWord(isWord)
{ }

Trie::Trie()
{
    mRootNode = std::unique_ptr<TrieNode>(new TrieNode("", false));
}

void Trie::insert(const std::string& word)
{
    // Do not check if word is already added (won't be the case in this
    // project)
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

    auto node = findNodeInSetStartingWith(parent->childrenWords, prefix);
    TrieNode* newParent = NULL;
    if (node == parent->childrenWords.end())
    {
        std::unique_ptr<TrieNode> newNode(new TrieNode(prefix, false));
        parent->childrenWords.push_back(std::move(newNode));
        newParent = parent->childrenWords.back().get();
    }
    else if (prefix != (*node)->word)
    {
        // Since the trie optimises space by not creating intermediate nodes
        // with just one child, it is possible that some reorganising of the
        // trie is necessary at this point.
        std::unique_ptr<TrieNode> newNode(new TrieNode(prefix, false));
        newNode->childrenWords.push_back(std::move(*node));
        parent->childrenWords.erase(node);
        parent->childrenWords.push_back(std::move(newNode));
        newParent = parent->childrenWords.back().get();
    }
    else
    {
        newParent = node->get();
    }


    recursiveInsertion(newParent, word);
}

TrieNode* Trie::recursiveFindNode(TrieNode* parent, const std::string& word) const
{
    if (parent->word == word)
    {
        return parent;
    }

    size_t parentLength = parent->word.size();
    std::string prefix = word.substr(0, parentLength+1);

    auto nextNode = findNodeInSetStartingWith(parent->childrenWords, prefix);

    if (nextNode != parent->childrenWords.end())
    {
        return recursiveFindNode(nextNode->get(), word);
    }
    else
    {
        return NULL;
    }
}

bool Trie::isValidWordInDictionary(const std::string& word) const
{
    TrieNode* node = recursiveFindNode(mRootNode.get(), word);
    return node != NULL && node->isValidWord;
}

bool Trie::isPrefixOfOtherWords(const std::string& prefix) const
{
    TrieNode* node = recursiveFindNode(mRootNode.get(), prefix);
    return node != NULL && !node->childrenWords.empty();
}
