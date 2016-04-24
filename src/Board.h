#include <cstddef>
#include <string>
#include <stack>

#define HORIZONTAL_SIZE 4
#define VERTICAL_SIZE 4

enum WordMultiplier {
    SINGLE_WORD = 1,
    DOUBLE_WORD = 2,
    TRIPLE_WORD = 3
};

enum LetterMultiplier {
    SINGLE_LETTER = 1,
    DOUBLE_LETTER = 2,
    TRIPLE_LETTER = 3
};

class Coordinate {
    public:
        Coordinate(int x, int y) : mX(x), mY(y) {  }
        int x() const { return mX; }
        int y() const { return mY; }

        Coordinate left() const { return Coordinate(mX - 1, mY); }
        Coordinate topLeft() const { return Coordinate(mX - 1, mY -1); }
        Coordinate up() const { return Coordinate(mX, mY - 1); }
        Coordinate topRight() const { return Coordinate(mX + 1, mY - 1); }
        Coordinate right() const { return Coordinate(mX + 1, mY); }
        Coordinate bottomRight() const { return Coordinate(mX + 1, mY + 1); }
        Coordinate down() const { return Coordinate(mX, mY + 1); }
        Coordinate bottomLeft() const { return Coordinate(mX - 1, mY + 1); }

        bool isValid() const
        {
            return x() >= 0 &&
                x() < HORIZONTAL_SIZE &&
                y() >= 0 && 
                y() < VERTICAL_SIZE;
        }

    private:
        int mX, mY;
};

class Letter {
    public:
        Letter();
        Letter(const std::string& letter, int value, Coordinate coord);
        Letter(const std::string& letter, int value, Coordinate coord, WordMultiplier wordMult);
        Letter(const std::string& letter, int value, Coordinate coord, LetterMultiplier letterMult);
        Letter(const std::string& letter, int value, Coordinate coord, 
                WordMultiplier wordMult, LetterMultiplier letterMult);

        const std::string& letter() const;
        int value() const;
        int wordMultiplier() const;
        const Coordinate& coord() const;
        bool available() const;
        void markAsUsed(bool used);

        std::string toString() const;

    private:
        std::string mLetter;
        int mValue;
        int mWordMultiplier;
        Coordinate mCoord;
        bool mAvailable;
};


class Word {
    public:
        Word();
        void addLetter(Letter* letter);
        void removeLastLetter();
        std::string currentWord() const { return mWord; }
        int score() const;
        std::string generateScreenCoords() const;
        bool operator<(const Word& rhs) const;

    private:
        int mScore;
        std::string mWord;
        std::stack<Letter*> mLetters;
        std::deque<Coordinate> mCoords;
        int mMultiplier;
};


class Board {
    public:
        Board(const std::string& fileName);
        bool isAvailable(const Coordinate& coord) const;
        Letter* letterAt(const Coordinate& coord);

        void print();

    private:
        Letter mLetters[VERTICAL_SIZE][HORIZONTAL_SIZE];
};
