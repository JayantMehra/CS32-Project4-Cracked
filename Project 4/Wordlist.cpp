#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <iostream>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> map;
    string computeLetterPattern(const string s) const;
    bool compare (const string s, const string k) const;
};

bool WordListImpl::loadWordList(string filename)
{
    
    /*
        The function loads words from the wordlist file and onto
        the custom defined map data structure.
     */
    
    map.reset();
    ifstream infile(filename);
    
    if (!infile)
        return false;
    
    string word;
    while (getline(infile, word)) {
        int i = 0;
        for (i = 0; i < word.size(); i++) {
            if ( !isalpha(word[i]) && (word[i] != '\''))
                break;
        }
        
        if (i == word.size()) {
            vector<string>* currentMapping = map.find(computeLetterPattern(word));
            
            if (currentMapping != nullptr) {
                (*currentMapping).push_back(word);
                map.associate(computeLetterPattern(word), *currentMapping);
                continue;
            }
            vector<string> temp;
            temp.push_back(word);
            map.associate(computeLetterPattern(word), temp);
        }
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    const vector<string>* temp = map.find(computeLetterPattern(word));
    
    if (temp != nullptr) {
        for (int i = 0; i < temp->size(); i++) {
            if (compare((*temp)[i], word))
                return true;
        }
    }
    
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    string pattern = computeLetterPattern(cipherWord);
    const vector<string>* temp = map.find(pattern);
    vector<string> candidates;
    
    for (int i = 0; i < cipherWord.size(); i++) {
        if (isalpha(currTranslation[i]) && !isalpha(cipherWord[i]))
            return candidates;
        if (currTranslation[i] == '?' && !isalpha(cipherWord[i]))
            return candidates;
        if (currTranslation[i] == '\'' && cipherWord[i] != '\'')
            return candidates;
    }
    
    if (temp != nullptr) {
        for (int i = 0; i < temp->size(); i++) {
            int j = 0;
            for (j = 0; j < currTranslation.size(); j++) {
                if (isalpha(currTranslation[j]) && tolower((*temp)[i][j]) != tolower(currTranslation[j]))
                    break;
            }
            if (j == currTranslation.size())
                candidates.push_back((*temp)[i]);
        }
    }
    
    return candidates;
}

string WordListImpl::computeLetterPattern(const string s) const {
    
    /*
     The function computes the letter pattern for the key.
     */
    
    
    std::string k;
    
    if (s.size() > 0) {
        
        k += 'A';
        char currentUniqueLetter = 'A';
        
        for (int i = 1; i < s.size(); i++) {
            
            if (s[i] == '\'') {
                k += '\'';
                continue;
            }
            
            int j = 0;
            for (j = 0; j <= i - 1; j++) {
                if (tolower(s[i]) == tolower(s[j])) {
                    k += k[j];
                    break;
                }
                
            }
            
            if (j == i)
                k += ++currentUniqueLetter;
        }
    }
    return k;
}

bool WordListImpl::compare (const std::string s, const std::string k) const {
    int size1 = s.size();
    int size2 = k.size();
    
    if (size1 != size2)
        return 0;
    
    for (int i = 0; i < size1; i++) {
        if (tolower(s[i]) != tolower(k[i]))
            return 0;
    }
    return 1;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}

