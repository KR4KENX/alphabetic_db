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
    string words;
};
string xorEncryptDecrypt(const std::string &input, const std::string &key);
bool compareIgnoringCapital(char a, char b);
void writeTableToDB(fstream& db, vector<string>& words, string userSecret);
dbTable readTableFromDB(fstream& db);
void sortWordsV(vector<string>& words);
vector<dbTable> readAllTablesFromDB(fstream& db);



#endif