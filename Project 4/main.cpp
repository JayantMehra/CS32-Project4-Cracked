#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <random>
#include <algorithm>
#include <numeric>
#include <cassert>
using namespace std;

const string WORDLIST_FILE = "/Users/jayantmehra/Desktop/Project 4/Project 4/wordlist.txt";

string encrypt(string plaintext)
{
    // Generate a to z
    char plaintextAlphabet[26+1];
    iota(plaintextAlphabet, plaintextAlphabet+26, 'a');
    plaintextAlphabet[26] = '\0';
    
    // Generate permutation
    string ciphertextAlphabet(plaintextAlphabet);
    default_random_engine e((random_device()()));
    shuffle(ciphertextAlphabet.begin(), ciphertextAlphabet.end(), e);
    
    // Run translator (opposite to the intended direction)
    Translator t;
    t.pushMapping(plaintextAlphabet, ciphertextAlphabet);
    return t.getTranslation(plaintext);
}

bool decrypt(string ciphertext)
{
    Decrypter d;
    if ( ! d.load(WORDLIST_FILE))
    {
        cout << "Unable to load word list file " << WORDLIST_FILE << endl;
        return false;
    }
    for (const auto& s : d.crack(ciphertext))
        cout << s << endl;
    return true;
}

int main(int argc, char* argv[])
{
    /*
    if (argc == 3  &&  argv[1][0] == '-')
    {
        switch (tolower(argv[1][1]))
        {
            case 'e':
                cout << encrypt(argv[2]) << endl;
                return 0;
            case 'd':
                if (decrypt(argv[2]))
                    return 0;
                return 1;
        }
    }
    */
    WordList w;
    assert(w.loadWordList(WORDLIST_FILE));
    assert(w.contains("grotto"));
    cout << "Passed All Tests." << endl;
    
    vector<string> temp = w.findCandidates("xyqbbq", "???mm?");
    
    for (int i = 0; i < temp.size(); i++)
        cout << temp[i] << endl;
    
    return 1;
}

