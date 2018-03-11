#include "provided.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    unordered_map<char, int> m_separators;
    mutable vector<string> words;
    
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++) {
        std::pair<char, int> separator (separators[i], i);
        m_separators.insert(separator);
    }
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    string currentWord = "";
    for (int i = 0; i < s.size(); i++) {
        if (m_separators.find(s[i]) == m_separators.end()) {
            currentWord += s[i];
        }
        else {
            words.push_back(currentWord);
            currentWord = "";
            while (i < s.size() && m_separators.find(s[i]) != m_separators.end()) {
                i++;
            }
            if (i != s.size())
                i--;
        }
    }
    return words;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

