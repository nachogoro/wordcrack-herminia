#include <vector>
#include <set>
#include <map>
#include <string>

class Dictionary;

typedef std::vector<std::string>::const_iterator dict_iterator;

class SubDict {
    public:
        SubDict();

        SubDict(std::pair<dict_iterator, dict_iterator> range, const std::string& prefix);

        bool isEmpty() const { return mRange.first == mRange.second; }

        bool contains(const std::string& word) const;

        bool moreWordsBeginningWith(const std::string& word) const;

        std::pair<dict_iterator, dict_iterator> range() { return mRange; }

    private:
        std::string mPrefix;
        std::pair<dict_iterator, dict_iterator> mRange;
};

class Dictionary {
    public:
        Dictionary(const std::string& fileName);

        bool wordExists(const std::string& word) const;

        bool moreWordsBeginningWith(const std::string& word) const;

    private:
        std::vector<std::string> mDict;
        mutable std::map<std::string, SubDict> mSubdicts;

};
