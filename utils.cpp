#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
using namespace std;
bool compareIgnoringCapital(char a, char b) {
    return std::tolower(a) < std::tolower(b);
}

void sortWordsV(vector<string>& words){
    sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compareIgnoringCapital);
    });
}

void writeTableToDB(fstream& db, vector<string>& words, string userSecret){
    dbTable dbTable;
    sortWordsV(words);
    dbTable.secret = hash<string>{}(userSecret);
    dbTable.words = words;

    // Zapisanie rozmiaru hasha
    size_t hashSize = sizeof(dbTable.secret);
    db.write(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));

    // Zapisanie hasha
    db.write(reinterpret_cast<char*>(&dbTable.secret), hashSize);

    // Zapisanie rozmiaru wektora
    size_t vectorSize = words.size();
    db.write(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

    // Zapisanie każdego słowa
    for (const string& word : words) {
        size_t size=word.size();
        db.write(reinterpret_cast<char*>(&size),sizeof(size));
        db.write(&word[0], size);
    }
}

dbTable readTableFromDB(fstream& db) {
    dbTable dbTable;

    // Odczytanie sekretu
    size_t secretSize;
    db.read(reinterpret_cast<char*>(&secretSize), sizeof(secretSize));
    dbTable.secret.resize(secretSize);
    db.read(&dbTable.secret[0], secretSize);

    // Odczytanie rozmiaru wektora
    size_t vectorSize;
    db.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

    // Odczytanie każdego słowa
    dbTable.words.resize(vectorSize);
    for (size_t i = 0; i < vectorSize; ++i) {
        size_t wordSize;
        db.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
        dbTable.words[i].resize(wordSize);
        db.read(&dbTable.words[i][0], wordSize);
    }
    return dbTable;
}

vector<dbTable> readAllTablesFromDB(fstream& db) {
    vector<dbTable> tables;

    while (true) {
        if (db.tellg() == 225) break;
        // Próba odczytania kolejnej struktury z pliku
        dbTable table = readTableFromDB(db);
        tables.push_back(table); // Dodanie odczytanej struktury do wektora
    }

    return tables;
}

// Currently - reading only one structure from file, words stored in pure text
// TODO - loop through dbTable objects, hash words

/*
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
}*/