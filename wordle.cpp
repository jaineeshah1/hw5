// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"

using namespace std;


// Add prototypes of helper functions here
void generateAll(
    const std::string& in,
    //const std::string& floating,
    std::map<char, int> floating,
    std::set<std::string>& ret,
    unsigned int remainingAZ,
    unsigned int startingIndex
);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict
)
{
    // Add your code here
    std::set<std::string> ret;

    unsigned int rem = 0;
    for (auto itr : in) {
        if (itr == '-') {
            rem += 1;
        }
    }

    unsigned int floatingSize = 0;
    std::map<char, int> floatingDict;
    for (auto itr : floating) {
        if (floatingDict.count(itr) == 0) {
            floatingDict[itr] = 1;
        }
        else {
            floatingDict[itr] += 1;
        }
        floatingSize += 1;
    }
    
    generateAll(in, floatingDict, ret, rem, 0);
    std::set<std::string> ret_filtered = std::set<std::string>(ret);

    for (auto itr : ret)
    {
        if (dict.count(itr) == 0) {
            ret_filtered.erase(itr);
        }
    }
    return ret_filtered;
}

const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

unsigned int getFloatingSize(std::map<char, int> &floating)
{
    int i = 0;
    for (std::pair<char, int> c : floating) {
        i += c.second;
    }
    return i;
}

// Define any helper functions here
void generateAll(
    const std::string& in,
    std::map<char, int> floating,
    std::set<std::string>& ret,
    unsigned int remainingAZ,
    unsigned int startingIndex
)
{
    //in: --ll
    // floating: bi
    bool bAtEnd = true;
    for (unsigned int i = startingIndex; i < in.length(); ++i) {
        char letter = in[i];
        if (letter == '-') {
            // found first character to change
            
            std::string newWord = in.substr(0, i);

            //if (remainingAZ == floating.size()) {
            if (remainingAZ == getFloatingSize(floating)) {
                for (std::pair<char, int> c : floating) {
                    std::string word = newWord;
                    word.push_back(c.first);
                    word += in.substr(i + 1);
                    
                    std::map<char, int> floating2 = std::map<char, int>(floating);
                    floating2[c.first] -= 1;
                    if (floating2[c.first] == 0) {
                        floating2.erase(c.first);
                    }
                    generateAll(word, floating2, ret, remainingAZ - 1, i + 1);
                }
                continue;
            }
            
            for (char c : alphabet) {
                std::string newWord = in.substr(0, i);

                std::map<char, int> floating2 = std::map<char, int>(floating);

                if (floating2.count(c) > 0) {
                    floating2[c] -= 1;
                    if (floating2[c] == 0) {
                        floating2.erase(c);
                    }
                }
                
                newWord.push_back(c);
                newWord += in.substr(i + 1);
                generateAll(newWord, floating2, ret, remainingAZ - 1, i + 1);
            }
            bAtEnd = false;
            break;
        }
    }
    // if at end of the word, add to ret
    // only if word has all floating characters
    if (bAtEnd && remainingAZ == 0) {
        ret.insert(in);
    }
}