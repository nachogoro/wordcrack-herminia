#include "Board.h"
#include "StringUtils.h"
#include "ScreenDimensions.h"

#include <stdexcept>
#include <fstream>
#include <cctype>
#include <iostream>

Letter::Letter()
    : Letter("", 0, Coordinate(-1, -1), SINGLE_WORD, SINGLE_LETTER)
{  }

Letter::Letter(const std::string& letter, int value, Coordinate coord)
    : Letter(letter, value, coord, SINGLE_WORD, SINGLE_LETTER)
{ }

Letter::Letter(const std::string& letter, int value, Coordinate coord, WordMultiplier wordMult)
    : Letter(letter, value, coord, wordMult, SINGLE_LETTER)
{ }


Letter::Letter(const std::string& letter, int value, Coordinate coord, LetterMultiplier letterMult)
    : Letter(letter, value, coord, SINGLE_WORD, letterMult)
{ }

Letter::Letter(const std::string& letter, int value, Coordinate coord, 
        WordMultiplier wordMult, LetterMultiplier letterMult)
    : mLetter(letter),
      mValue(letterMult * value),
      mWordMultiplier(wordMult),
      mCoord(coord),
      mAvailable(true)
{ }

const std::string& Letter::letter() const { return mLetter; }

int Letter::value() const { return mValue; }

int Letter::wordMultiplier() const { return mWordMultiplier; }

const Coordinate& Letter::coord() const { return mCoord; }

bool Letter::available() const { return mAvailable; }

void Letter::markAsUsed(bool used) {
    mAvailable = !used;
}

Word::Word() 
    : mScore(0),
      mWord(""),
      mLetters(std::stack<Letter*>()),
      mMultiplier(1)
{ }

void Word::addLetter(Letter* letter)
{
    letter->markAsUsed(true);
    mLetters.push(letter);
    mCoords.push_front(letter->coord());
    mWord += letter->letter();
    mScore = mScore + letter->value();
    mMultiplier *= letter->wordMultiplier();
}

void Word::removeLastLetter()
{
    Letter* toRemove = mLetters.top();
    mLetters.pop();
    mCoords.pop_front();
    mWord = mWord.substr(0, mWord.length() - toRemove->letter().length());
    mScore = mScore - toRemove->value();
    mMultiplier /= toRemove->wordMultiplier();
    toRemove->markAsUsed(false);
}

int Word::score() const
{ 
    const int base = mScore * mMultiplier;

    int lengthExtra = 0;

    switch (mLetters.size()) {
        case 4:
            lengthExtra = 2;
            break;
        case 5:
            lengthExtra = 4;
            break;
        case 6:
            lengthExtra = 8;
            break;
        case 7:
            lengthExtra = 12;
            break;
        case 8:
            lengthExtra = 18;
            break;
        default:
            break;
    }

    return base + lengthExtra;
}

std::string Word::generateScreenCoords() const {
    std::deque<Coordinate> coords(mCoords);
    std::string result = "";

    while (!coords.empty()) {
        Coordinate c = coords.back();
        coords.pop_back();

        result += std::to_string(ORIG_COORD_X + c.x()*X_GAP) + " ";
        result += std::to_string(ORIG_COORD_Y + c.y()*Y_GAP) + " ";
    }
    return result;
}

bool Word::operator<(const Word& rhs) const
{
    return mWord < rhs.currentWord();
}



Board::Board(const std::string& fileName)
{
    std::ifstream inputfile(fileName);

    std::string line;
    int y = 0;

    while (std::getline(inputfile, line))
    {
        if (y >= VERTICAL_SIZE) {
            throw std::runtime_error("Input file is too long");
        }

        std::vector<std::string> tokens = split(line, ' ');

        if (tokens.size() > HORIZONTAL_SIZE) {
            throw std::runtime_error("Line in input file is too long");
        }

        for (size_t x = 0; x < tokens.size(); ++x)
        {
            const std::string& token = tokens[x];
            const char tmp = token.at(0);
            std::string letter;
            if (tmp == 'Q') {
                letter = "qu";
            } else {
                letter = ::tolower(tmp);
            }
            const int value = token.at(1) - '0';

            WordMultiplier wordMult = SINGLE_WORD;
            LetterMultiplier letterMult = SINGLE_LETTER;

            if (token.length() == 4)
            {
                const std::string& modifier = token.substr(2);

                if (modifier == "DW") {
                    wordMult = DOUBLE_WORD;
                } else if (modifier == "TW") {
                    wordMult = TRIPLE_WORD;
                } else if (modifier == "DL") {
                    letterMult = DOUBLE_LETTER;
                } else if (modifier == "TL") {
                    letterMult = TRIPLE_LETTER;
                }
            }

            mLetters[y][x] = Letter(letter, value, Coordinate(x, y), 
                                        wordMult, letterMult);
        }

        ++y;
    }
}

Letter* Board::letterAt(const Coordinate& coord)
{
    return &(mLetters[coord.x()][coord.y()]);
}

bool Board::isAvailable(const Coordinate& coord) const
{
    return mLetters[coord.x()][coord.y()].available();
}

std::string Letter::toString() const
{
    std::string s = letter();
    s += std::to_string(value());
    s += "-";
    s += std::to_string(wordMultiplier());
    return s;
}

void Board::print()
{
    for (int i = 0; i < VERTICAL_SIZE; ++i) {
        for (int j = 0; j < HORIZONTAL_SIZE; ++j) {
            std::cout << mLetters[i][j].toString() << " ";
        }
        std::cout << std::endl;
    }
}
