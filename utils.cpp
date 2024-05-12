#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool compareIgnoringCapital(char a, char b) {
    return std::tolower(a) < std::tolower(b);
}

void sortWordsV(vector<string>& words){
    sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compareIgnoringCapital);
    });
}

void write(fstream& db, vector<string>& words){
    vector<string> v_words = read(db);
    for(string word: words){
        v_words.push_back(word);
    }
    sortWordsV(v_words);
    db.seekp(0, ios::beg);

    size_t size = v_words.size();
    db.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const string& word : v_words) {
        size_t size=word.size();
        db.write(reinterpret_cast<char*>(&size),sizeof(size));
        db.write(&word[0], size);
    }
}
vector<string> read(fstream& db) {
    vector<string> words;

    db.seekg(0, ios::end);
    size_t fileSize = db.tellg();
    db.seekg(0, ios::beg);

    if (fileSize == 0)
        return words;

    size_t size;
    db.read(reinterpret_cast<char*>(&size), sizeof(size));

    for (size_t i = 0; i < size; ++i) {
        std::string str;
        size_t size;
        db.read(reinterpret_cast<char*>(&size), sizeof(size));
        str.resize(size);
        db.read(&str[0], size);
        size_t str_size;
        words.push_back(str);
    }
    return words;
}