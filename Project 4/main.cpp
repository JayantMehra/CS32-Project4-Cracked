/*
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

const string WORDLIST_FILE = "/Users/jayantmehra/Desktop/Project 4/Project 4/tempwordlist.txt";

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
 */
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
    /*
    WordList w;
    assert(w.loadWordList(WORDLIST_FILE));
    assert(w.contains("grotto"));
    
    
    vector<string> temp = w.findCandidates("xyqbbq", "??????");
    
    for (int i = 0; i < temp.size(); i++)
        cout << temp[i] << endl;
    
    cout << "xyqbbq" << "??????" << endl;
     */
    /*
    Translator t;
    t.pushMapping("DHL", "ERD");
    assert(!t.pushMapping("QX", "RY"));
    assert(t.getTranslation("HDX")=="RE?");
    t.popMapping();
    assert(t.pushMapping("H", "S"));
    assert(!t.pushMapping("DHL", "ERD"));
    assert(t.pushMapping("A", "O"));
    assert(t.getTranslation("AHX") == "OS?");
    t.popMapping();
    assert(t.getTranslation("AHX") == "?S?");
    */
    /*
    Decrypter d;
    d.load(WORDLIST_FILE);
    vector<string> s = d.crack("smxsdg SGOSDG gfvgy!!");
    
    
    cout << s.size() << endl;
    for (int i = 0; i < s.size(); i++) {
        cout << s[i] << endl;
    }
    
    cout << "Passed All Tests." << endl;
    */
    /*
    Tokenizer t(" 12356789,;:.!()[]{}-\"#$%^&");
    vector<string> s = t.tokenize("Hello, World! I am Jayant Mehra & I will be your host tonight");
    cout << s.size() << endl;
    for (int i = 0; i < s.size(); i++)
        cout << s[i] << endl;
    */
    
    /*
    Translator t;
    t.pushMapping("ABC", "XYZ");
    assert ( ! t.pushMapping("abc", "lmn"));
    assert ( ! t.pushMapping("a", "q"));
    assert ( ! t.pushMapping("d", "x"));
    assert ( t.pushMapping("LMN", "dvl"));
    t.popMapping();
    //t.popMapping();
    assert( ! t.pushMapping("abc", "lmn"));
    cout << "Translator seems to be working" << endl;
    */
/*
    return 1;
}
*/

#include "provided.h"
#include "MyHash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cassert>
using namespace std;

// **************************************************************
//
// CHANGE TO false THE VARIABLES CORRESPONDING TO CLASSES YOU ARE *NOT*
// YET READY TO TEST

const bool shouldTestMyHash = true;
const bool shouldTestTokenizer = true;
const bool shouldTestWordList = true;
const bool shouldTestTranslator = true;
const bool shouldTestDecrypter = true;

// **************************************************************

const char* FILENAME = "sanityfile";

void testMyHash();
void testTokenizer();
void testWordList();
void testTranslator();
void testDecrypter();
bool createWordListFile();

int main()
{
    if (shouldTestMyHash    ) { testMyHash();     cout << "Passed MyHash tests" << endl; }
    if (shouldTestTokenizer ) { testTokenizer();  cout << "Passed Tokenizer tests" << endl; }
    if (shouldTestWordList  ) { testWordList();   cout << "Passed WordList tests" << endl; }
    if (shouldTestTranslator) { testTranslator(); cout << "Passed Translator tests" << endl; }
    if (shouldTestDecrypter ) { testDecrypter();  cout << "Passed Decrypter tests" << endl; }
}

void testMyHash()
{
    MyHash<int, string> m;
    m.associate(222, "Ethel");
    m.associate(333, "Lucy");
    m.associate(111, "Fred");
    assert(m.getNumItems() == 3);
    assert(abs(m.getLoadFactor() - 0.03) < 0.00001);
    string* s = m.find(333);
    assert(s != nullptr  &&  *s == "Lucy");
    m.associate(333, "Ricky");
    assert(m.getNumItems() == 3);
    const MyHash<int, string>& cm = m;
    const string* cs = cm.find(333);
    assert(cs != nullptr  &&  *cs == "Ricky");
    int x = 1776;
    for (int k = 0; k < 1000; k++)
    {
        m.associate(x, "Carey");
        x += 1789;
    }
    assert(m.getNumItems() == 1003);
    double lf = m.getLoadFactor();
    assert(lf >= 0.5/2  &&  lf <= 0.5);
    m.reset();
    m.associate(444, "David");
    assert(m.getNumItems() == 1);
    assert(abs(m.getLoadFactor() - 0.01) < 0.00001);
}

void testTokenizer()
{
    Tokenizer t(".#");
    vector<string> v = t.tokenize("this.is##.#a#SILLY test#..");
    const char* expected[4] = { "this", "is", "a", "SILLY test" };
    assert(v.size() == 4  && equal(v.begin(), v.end(), expected));
}

void testWordList()
{
    if ( ! createWordListFile())
    {
        cout << "Cannot complete WordList test" << endl;
        exit(1);
    }
    WordList wl;
    assert(wl.loadWordList(FILENAME));
    assert(wl.contains("expel"));
    assert(!wl.contains("extol"));
    vector<string> v = wl.findCandidates("wpxWv", "?x???");
    const char* expected[] = { "excel", "expel" };
    assert(v.size() == 2);
    for (auto& w : v)
        transform(w.begin(), w.end(), w.begin(), [](char c) { return tolower(c); });
    assert(equal(v.begin(), v.end(), expected) || equal(v.rbegin(), v.rend(), expected));
    v = wl.findCandidates("wpxwv", "?????");
    assert(v.size() == 4);
    v = wl.findCandidates("wpxwv", "?s??y");
    assert(v.size() == 0);
    remove(FILENAME);
}

void testTranslator()
{
    const char* q5bang = "?????" "!";  // Avoid trigraph
    Translator t;
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert( ! t.popMapping());
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert(t.pushMapping("dhL", "Erd"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert( ! t.pushMapping("QX", "RY"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert( ! t.pushMapping("H", "S"));
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert(t.pushMapping("QX", "AY"));
    assert(t.getTranslation("Hdqlx!") == "Ready!");
    assert(t.popMapping());
    assert(t.getTranslation("Hdqlx!") == "Re?d?!");
    assert(t.popMapping());
    assert(t.getTranslation("Hdqlx!") == q5bang);
    assert( ! t.popMapping());
}

void testDecrypter()
{
    if ( ! createWordListFile())
    {
        cout << "Cannot complete Decrypter test" << endl;
        exit(1);
    }
    Decrypter d;
    assert(d.load(FILENAME));
    vector<string> v = d.crack("smxsdg SGOSDG gfvgx!!");
    const char* expected[] = {
        "ablate AERATE excel!!",
        "ablate AERATE expel!!",
        "purple PEOPLE eater!!",
        "purple PEOPLE ether!!"
    };

    assert(v.size() == 4  &&  equal(v.begin(), v.end(), expected));
    assert(d.crack("smxsdg SGOSDG gfvgy!!").empty());
    remove(FILENAME);
}

bool createWordListFile()
{
    ofstream ofs(FILENAME);
    if ( ! ofs)
    {
        cout << "Cannot create word list file named " << FILENAME << endl;
        return false;
    }
    const char* words[] = {
        "people", "aerate", "ether", "essay", "eater",
        "excel", "Expel", "ablate", "purple"
    };
    for (auto w : words)
        ofs << w << endl;
    return true;
}

