#include "provided.h"
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    struct node {
        unordered_map<char, char> mappingTable;
        unordered_map<char, char> reverseMappingTable;
        node* next;
    };
    node* head;
    int stackSize;
};

TranslatorImpl::TranslatorImpl() {
    
    node* newNode = new node;
    
    
    
    char ch = 'A';
    for (int i = 0; i < 26; i++) {
        pair<char, char> map(ch+i, '?');
        newNode->mappingTable.insert(map);
    }
    
    head = newNode;
    head->next = nullptr;
    stackSize = 1;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if (ciphertext.size() != plaintext.size())
        return false;
    
    for (int i = 0; i < ciphertext.size(); i++)
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
            return false;
    
    for (int i = 0; i < ciphertext.size(); i++) {
        if (head->mappingTable.find(ciphertext[i]) != head->mappingTable.end() && isalpha(head->mappingTable.find(ciphertext[i])->second) && toupper(head->mappingTable.find(ciphertext[i])->second) != toupper(plaintext[i]))
            return false;
    }
    
    for (int i = 0; i < plaintext.size(); i++) {
        if (head->reverseMappingTable.find(plaintext[i]) != head->reverseMappingTable.end() && toupper(head->reverseMappingTable.find(plaintext[i])->second) != toupper(ciphertext[i]))
            return false;
    }
    
    
    node* newNode = new node;
    
    newNode->mappingTable = head->mappingTable;
    newNode->reverseMappingTable = head->reverseMappingTable;
   
    
    for (int i = 0; i < ciphertext.size(); i++) {
        newNode->mappingTable.find(ciphertext[i])->second = toupper(plaintext[i]);
        
        pair<char, char> reverseMap(toupper(plaintext[i]), ciphertext[i]);
        newNode->reverseMappingTable.insert(reverseMap);
    }
    
    newNode->next = head;
    head = newNode;
    stackSize++;

    return true;
    
}

bool TranslatorImpl::popMapping()
{
    if (stackSize == 1)
        return false;
    
    node* temp = head;
    head = head->next;
    delete temp;
    
    stackSize--;
    
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translatedString;
    
    for (int i = 0; i < ciphertext.size(); i++) {
        if (!isalpha(ciphertext[i]))
            translatedString += ciphertext[i];
        else {
            if (isupper(ciphertext[i])) {
                translatedString += head->mappingTable.find(ciphertext[i])->second;
            }
            else {
                if (isalpha(head->mappingTable.find(ciphertext[i])->second)) {
                    translatedString += tolower(head->mappingTable.find(ciphertext[i])->second);
                }
                else {
                    translatedString += '?';
                }
            }
        }
    }
    
    return translatedString;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}

