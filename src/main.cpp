#include "Board.h"
#include "Dictionary.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

// Prints the usage of the program
void printUsage(const std::string& programName);

// Valdidates the files used. Throws if they are not available
void validateConfig(
        const std::string&, const std::string&,
        const std::string&, const std::string&);

// Method to be executed by a thread in charge of finding all the words
// starting with the letters of row
void findAllWordsThread(
        Board board, const Dictionary& dict,
        std::shared_ptr<std::map<Word, int>> results, int row);

// Stores a new result in the map passed to it
void resultFound(std::shared_ptr<std::map<Word, int>> results, Word& word);

// Given a new Word word and the Coordinate coord of the last letter added to
// it, it recursively finds all the words which can be formed from it.
void processNewWord(
        Board& board, const Dictionary& dict, 
        std::shared_ptr<std::map<Word, int>> results,
        const Coordinate& coord, Word word);

// Determines whether more words could potentially be formed from the current
// Word word and a new letter in Coordinate coord. Helper method to
// processNewWord(), in charge of recursion
void verifyWord(
        Board& board, const Dictionary& dict,
        std::shared_ptr<std::map<Word, int>> results,
        const Coordinate& coord, Word word);

// Writes the results to the appropriate files
void writeResultsToOutputFiles(
        std::shared_ptr<std::map<Word,int>> results,
        const std::string& outFileWords,
        const std::string& outFileCoords);

int main(int argc, char** argv)
{
    std::string programName(argv[0]);
    if (argc != 9) {
        printUsage(programName);
        return 0;
    }

    std::string dictFileName;
    std::string boardFileName;
    std::string outFileWords;
    std::string outFileCoords;

    // Get the configuration parameters
    for (int i = 1; i <= 7; i+=2)
    {
        std::string config(argv[i]);
        if (config == "--dict") {
            dictFileName = argv[i+1];
        } else if (config == "--board") {
            boardFileName = argv[i+1];
        } else if (config == "--outwords") {
            outFileWords = argv[i+1];
        } else if (config == "--outcoords") {
            outFileCoords = argv[i+1];
        } else {
            printUsage(programName);
            return 0;
        }
    }

    // Validate the configuration parameters
    validateConfig(dictFileName, boardFileName, outFileWords, outFileCoords);

    Dictionary dict(dictFileName);
    Board board(boardFileName);
    std::shared_ptr<std::map<Word, int>> foundWords(new std::map<Word,int>());

    std::chrono::high_resolution_clock::time_point start =
        std::chrono::high_resolution_clock::now();

    // One thread per row
    std::thread threads[VERTICAL_SIZE];
    for (int row = 0; row < VERTICAL_SIZE; ++row)
    {
        threads[row] = std::thread(findAllWordsThread, board, dict, foundWords, row);
    }

    // Join the threads with the main one
    for (int i = 0; i < VERTICAL_SIZE; ++i)
    {
        threads[i].join();
    }

    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "All words found in " << duration << " seconds" << std::endl;


    writeResultsToOutputFiles(foundWords, outFileWords, outFileCoords);
}

void printUsage(const std::string& programName)
{
    std::cerr << "Usage: " << programName << "--dict <dictionary> --board <board> --outwords <words> --outcoords <coords>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Where: " << std::endl;
    std::cerr << "<dictionary>: text file containing all the valid words in the desired language" << std::endl;
    std::cerr << std::endl;
    std::cerr << "<board>: text file containing the board to solve, including values of letters and modifiers" << std::endl;
    std::cerr << "         in the form of <letter><value><modifier_if_any>" << std::endl;
    std::cerr << "\t Example contents:" << std::endl;
    std::cerr << "\t I1 R1TW B3 N1" << std::endl;
    std::cerr << "\t O1 L1TL S1 C3" << std::endl;
    std::cerr << "\t T1 O1TL E1 V4" << std::endl;
    std::cerr << "\t A1 C3 A1 V4" << std::endl;
    std::cerr << std::endl;
    std::cerr << "<words>: output file to store the possible words and their score" << std::endl;
    std::cerr << std::endl;
    std::cerr << "<coords>: output file to store the coordinates in the screen of the possible words" << std::endl;
}

void validateConfig(
        const std::string& dict, const std::string& board,
        const std::string& outwords, const std::string& outcoords)
{
    if (!std::ifstream(dict.c_str()).good()) {
        throw std::runtime_error(dict + " does not exist");
    }

    if (!std::ifstream(board.c_str()).good()) {
        throw std::runtime_error(board + " does not exist");
    }

    if (!std::ofstream(outwords.c_str()).good()) {
        throw std::runtime_error(outwords + " can not be written to");
    }

    if (!std::ofstream(outcoords.c_str()).good()) {
        throw std::runtime_error(outcoords + " can not be written to");
    }
}


void findAllWordsThread(
        Board board, const Dictionary& dict,
        std::shared_ptr<std::map<Word, int> > results, int row)
{
    for (int col = 0; col < HORIZONTAL_SIZE; ++col)
    {
        Coordinate coord(col, row);
        Word word;
        word.addLetter(board.letterAt(coord));
        processNewWord(board, dict, results, coord, word);
        word.removeLastLetter();
    }
}

void resultFound(std::shared_ptr<std::map<Word, int> > results, Word& word)
{
    static std::mutex mapMutex;

    mapMutex.lock();
    auto wordInMap = results->begin();
    for (wordInMap = results->begin(); wordInMap != results->end(); ++wordInMap)
    {
        if (wordInMap->first.currentWord() == word.currentWord())
            break;
    }

    if ((wordInMap == results->end()) || (wordInMap->second < word.score())) {
        if (wordInMap != results->end()) {
            // Make sure we delete the entry so that the new one gets inserted
            results->erase(wordInMap);
        }
        results->insert(std::make_pair(word, word.score()));
    }
    mapMutex.unlock();
}


void processNewWord(Board& board, const Dictionary& dict, std::shared_ptr<std::map<Word, int> > results, const Coordinate& coord, Word word)
{
    Coordinate next = coord.left();
    verifyWord(board, dict, results, next, word);

    next = coord.topLeft();
    verifyWord(board, dict, results, next, word);

    next = coord.up();
    verifyWord(board, dict, results, next, word);

    next = coord.topRight();
    verifyWord(board, dict, results, next, word);

    next = coord.right();
    verifyWord(board, dict, results, next, word);

    next = coord.bottomRight();
    verifyWord(board, dict, results, next, word);

    next = coord.down();
    verifyWord(board, dict, results, next, word);

    next = coord.bottomLeft();
    verifyWord(board, dict, results, next, word);
}

void verifyWord(Board& board, const Dictionary& dict, std::shared_ptr<std::map<Word, int> > results, const Coordinate& coord, Word word)
{
    if (coord.isValid() && board.isAvailable(coord)) {
        word.addLetter(board.letterAt(coord));
        if (dict.wordExists(word.currentWord())) {
            resultFound(results, word);
        }

        if (dict.moreWordsBeginningWith(word.currentWord())) {
            processNewWord(board, dict, results, coord, word);
        }
        word.removeLastLetter();
    }
}

void writeResultsToOutputFiles(
        std::shared_ptr<std::map<Word,int> > foundWords,
        const std::string& outFileWords,
        const std::string& outFileCoords)
{
    // Sort results by score
    std::vector<std::pair<Word, int> > pairs;
    for (auto itr = foundWords->begin(); itr != foundWords->end(); ++itr) {
        pairs.push_back(*itr);
    }

    sort(pairs.begin(), pairs.end(), [=](const std::pair<Word, int>& a, const std::pair<Word, int>& b)
            {
            return a.second > b.second;
            }
        );

    std::ofstream wordOutput(outFileWords);
    std::ofstream coordOutput(outFileCoords);
    for (auto it = pairs.begin(); it != pairs.end(); ++it) {
        wordOutput << it->first.currentWord() << " " << it->second << std::endl;
        coordOutput << it->first.generateScreenCoords() << std::endl;
    }
}

