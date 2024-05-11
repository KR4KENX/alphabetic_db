#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


int main(int argc, char* argv[]){
    fstream db(argv[2], ios::in | ios::out | ios::binary);
    if(argc < 3){
        cout << "Operation not specified" << endl;
        return 1;
    }
    if(!db.good()){
        cout << "Error with opening db file" << endl;
        return 1;
    }
    if(argv[1][0] == 'w'){
        cout << "Write to database, add ENTER afer every sentence ... " << endl;
        vector<string> words;
        string word;
        while(getline(cin, word)){
            words.push_back(word);
        }
        sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compareIgnoringCapital);
        });
        write(db, words);
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
        // string word;
        // getline(cin, word);
        // if (read(in, word)) cout << "Word already in DB" << endl;
        // else cout << "Word not in DB" << endl;
    }
    db.close();
    return 0;
}