#include <vector>
#include <set>
#include <map>
#include <string>

#include "Trie.h"

class Dictionary;

class Dictionary {
    public:
        Dictionary(const std::string& fileName);

        bool wordExists(const std::string& word) const;

        bool moreWordsBeginningWith(const std::string& word) const;

    private:
        // Internal dictionary, stored as a Trie
        Trie mDict;
};
