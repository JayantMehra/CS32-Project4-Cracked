#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList words;
    Tokenizer tokens;
    Translator mappings;
    vector<string> plaintext;
    
    void generatePlaintext(const string& ciphertext, Translator& maps);
};

DecrypterImpl::DecrypterImpl(): tokens(" 12356789,;:.!()[]{}-\"#$%^&") {
    plaintext.erase(plaintext.begin(), plaintext.end());
};

bool DecrypterImpl::load(string filename)
{
    if (!words.loadWordList(filename))
        return false;
    
    return true;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    //vector<string> proposed
    plaintext.erase(plaintext.begin(), plaintext.end());
    generatePlaintext(ciphertext, mappings);
    sort(plaintext.begin(), plaintext.end());
    return plaintext;
}

void DecrypterImpl::generatePlaintext(const string &ciphertext, Translator &maps) {
    
    vector<string> ciphertextWords = tokens.tokenize(ciphertext);
    vector<string> currentTranslation;
    
    for (int i = 0; i < ciphertextWords.size(); i++)
        currentTranslation.push_back(maps.getTranslation(ciphertextWords[i]));
    
    int max = 0;
    int pos = 0;
    for (int i = 0; i < currentTranslation.size(); i++) {
        int counter = 0;
        for (int j = 0; j < currentTranslation[i].size(); j++)
            if (currentTranslation[i][j] == '?')
                counter++;
        if (counter > max) {
            max = counter;
            pos = i;
        }
    }
    
    vector<string> compatibleWords = words.findCandidates(ciphertextWords[pos], currentTranslation[pos]);
    
    if (compatibleWords.empty()) {
        maps.popMapping();
        return;
    }
   
    int i = 0;
    for (i = 0; i < compatibleWords.size(); i++) {

        //cout << pos << ":" <<compatibleWords[i] <<  endl;
        if ( !maps.pushMapping(ciphertextWords[pos], compatibleWords[i]) ) {
            //cout << "Mapping not permitted" << endl;
            continue;
        }
        int fullyTranslatedWords = 0;
        vector<string> proposedPlainText;
        
        for (int j = 0; j < ciphertextWords.size(); j++)
            proposedPlainText.push_back(maps.getTranslation(ciphertextWords[j]));
        
        int wrongMapping = 0;
        for (int j = 0; j < proposedPlainText.size(); j++) {
            int k = 0;
            for (k = 0; k < proposedPlainText[j].size(); k++) {
                if (proposedPlainText[j][k] == '?')
                    break;
            }
            
            if (k == proposedPlainText[j].size()) {
                if ( !words.contains(proposedPlainText[j]) ) {
                    maps.popMapping();
                    wrongMapping++;
                    break;
                }
                fullyTranslatedWords++;
            }
        }
        
        if ( wrongMapping )
            continue;
        
        if (fullyTranslatedWords != proposedPlainText.size())
            generatePlaintext(ciphertext, maps);
        
        if (fullyTranslatedWords == proposedPlainText.size()) {
            plaintext.push_back(maps.getTranslation(ciphertext));
            maps.popMapping();
            continue;
        }
    }
    maps.popMapping();
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}
