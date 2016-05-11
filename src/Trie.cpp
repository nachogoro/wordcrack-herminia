#include "Trie.h"

#include <algorithm>

TrieNode* findNodeInSetStartingWith(std::set<std::shared_ptr<TrieNode> > nodes, const std::string& word)
{
    auto node = std::find_if(nodes.begin(), nodes.end(),
            [word](std::shared_ptr<TrieNode> x) { return x->word.substr(0, word.size()) == word; }
            );
    if (node == nodes.end())
    {
        return NULL;
    }
    else
    {
        return node->get();
    }
}

Trie::Trie()
{
    mRootNode = std::shared_ptr<TrieNode>(new TrieNode("", false));
}

void Trie::insert(const std::string& word)
{
    if (word == "")
    {
        // Do nothing
        return;
    }

    TrieNode* node = findNode(mRootNode.get(), word);
    if (node != NULL)
    {
        node->isValidWord = true;
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

    TrieNode* node = findNodeInSetStartingWith(parent->childrenWords, prefix);
    if (node == NULL)
    {
        std::shared_ptr<TrieNode> newNode(new TrieNode(prefix, false));
        node = newNode.get();
        parent->childrenWords.insert(std::move(newNode));
    }
    else if (prefix != node->word)
    {
        // Since the trie optimises space by not creating intermediate nodes
        // with just one child, it is possible that some reorganising of the
        // trie is necessary at this point.
        auto nodeToMove = std::find_if(parent->childrenWords.begin(),
                parent->childrenWords.end(),
                [node](std::shared_ptr<TrieNode> x) {
                    return x.get() == node;
                }
                );
        std::shared_ptr<TrieNode> newNode(new TrieNode(prefix, false));
        newNode->childrenWords.insert(std::move(*nodeToMove));
        parent->childrenWords.erase(nodeToMove);
        node = newNode.get();
        parent->childrenWords.insert(std::move(newNode));
    }

    recursiveInsertion(node, word);
}

TrieNode* Trie::findNode(TrieNode* parent, const std::string& word) const
{
    if (parent->word == word)
    {
        return parent;
    }

    size_t parentLength = parent->word.size();
    std::string prefix = word.substr(0, parentLength+1);

    TrieNode* nextNode = findNodeInSetStartingWith(parent->childrenWords, prefix);

    if (nextNode)
    {
        return findNode(nextNode, word);
    }
    else
    {
        return NULL;
    }
}

bool Trie::isValidWordInDictionary(const std::string& word) const
{
    TrieNode* node = findNode(mRootNode.get(), word);
    return node != NULL && node->isValidWord;
}

bool Trie::isPrefixOfOtherWords(const std::string& prefix) const
{
    TrieNode* node = findNode(mRootNode.get(), prefix);
    return node != NULL && !node->childrenWords.empty();
}
