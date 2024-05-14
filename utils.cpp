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

string serialize(vector<string> words){
    string serializedWordsChain;
    for (const string& word : words) {
        serializedWordsChain += to_string(word.size())+ word;
    }
    return serializedWordsChain;
}

vector<string> deserialize(string serializedWord){
    vector<string> res;
    int i = 0;
    while (i < serializedWord.size()) {
        string hop = "";
        while (isdigit(serializedWord[i])){
            hop.push_back(serializedWord[i]);
            i++;
        }
        int ihop = atoi(hop.c_str());
        string actual = serializedWord.substr(i, ihop);
        res.push_back(actual);
        i = i + ihop;
    }
    return res;
}

void writeTableToDB(fstream& db, dbTable& dbTable){
    sortWordsV(dbTable.words);
    string serializedWordsChain = serialize(dbTable.words);

    string encrypted = xorEncryptDecrypt(serializedWordsChain, dbTable.secret);
    size_t wordsSize = encrypted.size();

    short wordsByteLenght = encrypted.size() + sizeof(wordsSize);

    size_t hash = hashSecret(dbTable.secret);

    size_t hashSize = sizeof(hash);
    db.write(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.write(reinterpret_cast<char*>(&hash), hashSize);

    db.write(reinterpret_cast<char*>(&wordsByteLenght), sizeof(wordsByteLenght));

    db.write(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
    db.write(encrypted.c_str(), wordsSize);
}

dbTable readTableFromDB(fstream& db, string secret) {
    dbTable dbTable;
    size_t hashSize;
    size_t hash;
    short hopSize;
    db.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.read(reinterpret_cast<char*>(&hash), sizeof(hash));
    db.read(reinterpret_cast<char*>(&hopSize), sizeof(hopSize));
    if(hashSecret(secret) != hash) {
        db.seekg(hopSize, ios::cur);
        return dbTable;
    }
    size_t wordsSize;
    db.read(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
   
    char* contentBuffer = new char[wordsSize];
    db.read(contentBuffer, wordsSize);

    string content(contentBuffer, wordsSize);
    
    delete[] contentBuffer;

    dbTable.words = deserialize(xorEncryptDecrypt(content, secret));
    
    return dbTable;
}

vector<dbTable> readAllTablesFromDB(fstream& db, string secret) {
    vector<dbTable> tables = {};
   while (db.good()) {
        dbTable table = readTableFromDB(db, secret);
        if(table.words.size() > 0) tables.push_back(table);
    }
    return tables;
}