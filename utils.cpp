#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
using namespace std;

size_t hashSecret(string secret){
    hash<string> hasher;
    size_t hash = hasher(secret);
    return hash;
}

string xorEncryptDecrypt(const string& input, const string& key) {
    string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key[i % key.size()]; // Operacja XOR na pojedynczym znaku
    }
    return output;
}

bool compareIgnoringCapital(char a, char b) {
    return std::tolower(a) < std::tolower(b);
}

void sortWordsV(vector<string>& words){
    sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compareIgnoringCapital);
    });
}

void writeTableToDB(fstream& db, dbTable& dbTable){
    sortWordsV(dbTable.words);
    string serializedWordsChain;
    short wordsByteLenght;
    for (const string& word : dbTable.words) {
        wordsByteLenght += word.size();
        serializedWordsChain += to_string(word.size())+ word;
    }
    size_t hash = hashSecret(dbTable.secret);

    string encrypted = xorEncryptDecrypt(serializedWordsChain, dbTable.secret);

    size_t hashSize = sizeof(hash);
    db.write(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.write(reinterpret_cast<char*>(&hash), hashSize);

    db.write(reinterpret_cast<char*>(&wordsByteLenght), sizeof(wordsByteLenght));

    size_t wordsSize = encrypted.size();
    db.write(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
    db.write(encrypted.c_str(), wordsSize);
}

dbTable readTableFromDB(fstream& db, string secret) {
    dbTable dbTable;
    size_t hashSize;
    size_t hash;
    short hopSize;
    db.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.read(reinterpret_cast<char*>(&hash), hashSize);
    db.read(reinterpret_cast<char*>(&hopSize), sizeof(hopSize));
    if(hashSecret(secret) != hash) {
        db.seekg(hopSize, ios::cur);
        return dbTable;
    }
    size_t wordsSize;
    db.read(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
   // Alokacja pamięci dla content
    char* contentBuffer = new char[wordsSize];
    db.read(contentBuffer, wordsSize);
    
    // Przypisanie wartości do zmiennej content
    string content(contentBuffer, wordsSize);
    
    // Zwolnienie zaalokowanej pamięci
    delete[] contentBuffer;

    cout << xorEncryptDecrypt(content, secret) << endl;
    
    return dbTable;
}

vector<dbTable> readAllTablesFromDB(fstream& db) {
    vector<dbTable> tables;
   /* while (EDIT tellg() != ios::end) {
        cout << "Next object: " << db.tellg() << endl;
        if (db.tellg() >= 255) break;
        // Próba odczytania kolejnej struktury z pliku
        dbTable table = readTableFromDB(db, );
        tables.push_back(table); // Dodanie odczytanej struktury do wektora
    }*/

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