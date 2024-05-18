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
    string encryptedTableName = xorEncryptDecrypt(dbTable.tableName, dbTable.secret);
    size_t wordsSize = encrypted.size();
    size_t tableNameSize = encryptedTableName.size();

    short wordsByteLenght = wordsSize + sizeof(wordsSize) + tableNameSize;

    size_t hash = hashSecret(dbTable.secret);

    size_t hashSize = sizeof(hash);
    db.write(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.write(reinterpret_cast<char*>(&hash), hashSize);

    db.write(reinterpret_cast<char*>(&wordsByteLenght), sizeof(wordsByteLenght));

    db.write(reinterpret_cast<char*>(&tableNameSize), sizeof(tableNameSize));
    db.write(encryptedTableName.c_str(), tableNameSize);

    db.write(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
    db.write(encrypted.c_str(), wordsSize);
}

dbTable readTableFromDB(fstream& db, string secret, string searchTable) {
    dbTable Table;
    size_t hashSize;
    size_t hash;
    short hopSize;
    db.read(reinterpret_cast<char*>(&hashSize), sizeof(hashSize));
    db.read(reinterpret_cast<char*>(&hash), sizeof(hash));
    db.read(reinterpret_cast<char*>(&hopSize), sizeof(hopSize));
    if(hashSecret(secret) != hash) {
        db.seekg(hopSize, ios::cur);
        return Table;
    }
    size_t tableNameSize;
    db.read(reinterpret_cast<char*>(&tableNameSize), sizeof(tableNameSize));
   
    char* tableNameBuffer = new char[tableNameSize];
    db.read(tableNameBuffer, tableNameSize);

    string tableName(tableNameBuffer, tableNameSize);
    
    delete[] tableNameBuffer;

    Table.tableName = xorEncryptDecrypt(tableName, secret);

    if(Table.tableName != searchTable && searchTable != "."){
        db.seekg((hopSize - tableNameSize), ios::cur);
        dbTable emptyTable;
        return emptyTable;        
    }
    //
    size_t wordsSize;
    db.read(reinterpret_cast<char*>(&wordsSize), sizeof(wordsSize));
   
    char* contentBuffer = new char[wordsSize];
    db.read(contentBuffer, wordsSize);

    string content(contentBuffer, wordsSize);
    
    delete[] contentBuffer;

    Table.words = deserialize(xorEncryptDecrypt(content, secret));
    
    return Table;
}

dbTable readTableFromDBSafe(fstream& db){
    
}

vector<dbTable> readAllTablesFromDB(fstream& db, string secret, string searchTable) {
    vector<dbTable> tables = {};
    while (db.good()) {
            dbTable table = readTableFromDB(db, secret, searchTable);
            if(table.words.size() > 0) tables.push_back(table);
        }
    return tables;
}

int modifyTable(fstream& db, string secret, string modifyTableName, vector<string> wordsToAdd, vector<string> wordsToDrop){
    dbTable tableToModify;
    vector<dbTable> temp = readAllTablesFromDB(db, secret, modifyTableName);
    if(temp.size() == 0){
        return 1;
    }
    tableToModify = temp[0];
    for (const string& word : wordsToDrop) {
    tableToModify.words.erase(
        std::remove(tableToModify.words.begin(), tableToModify.words.end(), word),
        tableToModify.words.end()
    );
    }
    for(const string& word : wordsToAdd){
        tableToModify.words.push_back(word);
    }
    return 0;
}