#ifndef UTILS_HPP_
#define UTILS_HPP_
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

struct dbTable {
    string secret;
    string tableName;
    vector<string> words;
};
size_t hashSecret(string secret);
string xorEncryptDecrypt(const std::string &input, const std::string &key);
bool compareIgnoringCapital(char a, char b);
void writeTableToDB(fstream& db, dbTable& dbTable);
dbTable readTableFromDB(fstream& db, string secret);
void sortWordsV(vector<string>& words);
string serialize(vector<string> words);
vector<string> deserialize(string serializedWord);
vector<dbTable> readAllTablesFromDB(fstream& db, string secret);



#endif