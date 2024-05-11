#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool biggerWord(string word1, string word2){
    for (char &c : word1)
        c = tolower(c);
    for (char &c : word2)
        c = tolower(c);
    // Word 1 > Word 2 = return True, Word 2 >= Word 1 = return False
    for(int i = 0; i < max(word1.size(), word2.size()); i++){
        if (i >= word1.size()) return false;
        if (i >= word2.size()) return true;
        if (word1[i] < word2[i]) return false;
        if (word1[i] > word2[i]) return true;
    }
    return false;
}
bool compareIgnoringCapital(char a, char b) {
    return std::tolower(a) < std::tolower(b);
}
bool linearSearchDB(ifstream& db, int r, string target){
    int l = 0;
    string line;
    while (l < r){
        getline(db, line);
        if (line == target) return true;
        if (biggerWord(line, target)) return false;
        l++;
    }
    return false;
}

void write(fstream& db, vector<string>& words){
    vector<string> v_words = read(db);
    for(string word: words){
        v_words.push_back(word);
    }
    db.seekp(0, ios::beg);
    // Write the size of the vector.
    size_t size = v_words.size();
    db.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // Write each string.
    for (const string& name : v_words) {
        // Write the size of the string.
        size = name.size();
        db.write(reinterpret_cast<const char*>(&size), sizeof(size));
        // Write the string data.
        db.write(name.c_str(), size);
    }
}
vector<string> read(fstream& db) {
    vector<string> names;

    // Get the size of the file
    db.seekg(0, ios::end);
    size_t fileSize = db.tellg();
    db.seekg(0, ios::beg);

    // If the file is empty, return empty vector
    if (fileSize == 0)
        return names;

    // Read the size of the vector.
    size_t size;
    db.read(reinterpret_cast<char*>(&size), sizeof(size));

    // Read each string.
    for (size_t i = 0; i < size; ++i) {
        // Read the size of the string.
        size_t str_size;
        db.read(reinterpret_cast<char*>(&str_size), sizeof(str_size));
        // Read the string data.
        string name(str_size, '\0');
        db.read(&name[0], str_size);
        names.push_back(name);
    }
    return names;
}

// TO DO - seraching for word
/*
bool read(ifstream& db, string word){
    int line_count = checkFileLenght(db, 0);
    return linearSearchDB(db, line_count, word);
}
*/