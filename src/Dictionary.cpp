#include "Dictionary.h"

#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <iterator>

#define PREFIX_SIZE 4

Dictionary::Dictionary(const std::string& fileName)
{
    std::ifstream infile(fileName);

    std::string line;
    while (std::getline(infile, line))
    {
        mDict.insert(line);
    }
}

bool Dictionary::wordExists(const std::string& word) const
{
    return mDict.isValidWordInDictionary(word);
}

bool Dictionary::moreWordsBeginningWith(const std::string& word) const
{
    return mDict.isPrefixOfOtherWords(word);
}

