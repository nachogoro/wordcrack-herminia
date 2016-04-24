#include "Dictionary.h"

#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <iterator>

#define PREFIX_SIZE 4

namespace
{
    bool compareStrings(const std::string& a, const std::string& b)
    {
        const unsigned int letters = a.length() > PREFIX_SIZE ? PREFIX_SIZE : a.length();
        return (a.substr(0, letters) < b.substr(0, letters));
    }
}

Dictionary::Dictionary(const std::string& fileName)
{
    std::ifstream infile(fileName);

    std::string line;
    while (std::getline(infile, line))
    {
        mDict.push_back(line);
    }
}

bool Dictionary::wordExists(const std::string& word) const
{
    const std::string& prefix = word.substr(0, PREFIX_SIZE);

    auto subdict = mSubdicts.find(prefix);

    SubDict dictToLookUp;

    if (subdict == mSubdicts.end())
    {
        // We need to create the subdictionary
        std::vector<SubDict> usefulSubdicts;

        for (unsigned int i = 1; i < PREFIX_SIZE; ++i) {
            auto usefulSubdict = mSubdicts.find(word.substr(0, i));

            if (usefulSubdict != mSubdicts.end()) {
                usefulSubdicts.push_back(usefulSubdict->second);
            } else {
                break;
            }
        }

        std::pair<dict_iterator, dict_iterator> boundaries;

        if (usefulSubdicts.empty()) {
            boundaries.first = mDict.begin();
            boundaries.second = mDict.end();
        } else {
            boundaries = usefulSubdicts[usefulSubdicts.size() - 1].range();
        }

        dictToLookUp = SubDict(boundaries, prefix);
        mSubdicts[prefix] = dictToLookUp;
    }
    else
    {
        dictToLookUp = subdict->second;
    }

    return dictToLookUp.contains(word);
}

bool Dictionary::moreWordsBeginningWith(const std::string& word) const
{
    // Assume there is already a valid subdictionary
    const std::string& prefix = word.substr(0, PREFIX_SIZE);
    return mSubdicts[prefix].moreWordsBeginningWith(word);
}

SubDict::SubDict()
{  }

SubDict::SubDict(std::pair<dict_iterator, dict_iterator> range, const std::string& prefix)
    : mPrefix(prefix)
{
    mRange = std::equal_range(range.first, range.second, prefix, ::compareStrings);
}

bool SubDict::contains(const std::string& word) const
{
    std::pair<dict_iterator, dict_iterator> itPair =
        std::equal_range(mRange.first, mRange.second, word);

    return itPair.first != itPair.second;
}

bool SubDict::moreWordsBeginningWith(const std::string& word) const
{
    std::pair<dict_iterator, dict_iterator> itPair =
        std::equal_range(mRange.first, mRange.second, word, ::compareStrings);

    return std::distance(itPair.first, itPair.second) > 1;
}
