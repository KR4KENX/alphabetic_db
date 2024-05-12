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

void writeTableToDB(fstream& db, vector<string>& words, string userSecret){
    dbTable dbTable;
    sortWordsV(words);
    string serializedWordsChain;
    short wordsByteLenght;
    for (const string& word : words) {
        wordsByteLenght += word.size();
        serializedWordsChain += to_string(word.size())+ word;
    }
    dbTable.secret = hash<string>{}(userSecret);

    string encrypted = xorEncryptDecrypt(serializedWordsChain, dbTable.secret);

    dbTable.words = encrypted;

    size_t hashSize = sizeof(dbTable.secret);
    db.write(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.write(reinterpret_cast<char*>(&dbTable.secret), hashSize);

    db.write(reinterpret_cast<char*>(&wordsByteLenght), sizeof(wordsByteLenght));

    size_t wordsSize = sizeof(dbTable.words);
    db.write(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
    db.write(reinterpret_cast<char*>(&dbTable.words), wordsSize);
}

dbTable readTableFromDB(fstream& db) {
    dbTable dbTable;
   /* // Odczytanie sekretu
    size_t secretSize;
    db.read(reinterpret_cast<char*>(&secretSize), sizeof(secretSize));
    dbTable.secret.resize(secretSize);
    cout << "Secret size: " << secretSize << endl;
    db.read(&dbTable.secret[0], secretSize);

    // Odczytanie rozmiaru wektora
    size_t vectorSize;
    db.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

    cout << "Vector size: " << vectorSize << endl;

    // Odczytanie każdego słowa
    dbTable.words.resize(vectorSize);
    for (size_t i = 0; i < vectorSize; ++i) {
        size_t wordSize;
        db.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
        dbTable.words[i].resize(wordSize);
        db.read(&dbTable.words[i][0], wordSize);
    }*/
    return dbTable;
    //przesuniecie o bajty działa licho, odczytaj hash secreta jeśli się nie zgadza po prostu nie zapisuj do tabeli db tylko czytaj
}

vector<dbTable> readAllTablesFromDB(fstream& db) {
    vector<dbTable> tables;
    while (true) {
        cout << "Next object: " << db.tellg() << endl;
        if (db.tellg() >= 255) break;
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