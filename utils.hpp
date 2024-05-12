#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
using namespace std;

struct dbTable {
    string secret;
    vector<string> words;
};

bool compareIgnoringCapital(char a, char b);
void writeTableToDB(fstream& db, vector<string>& words, string userSecret);
dbTable readTableFromDB(fstream& db);
void sortWordsV(vector<string>& words);
vector<dbTable> readAllTablesFromDB(fstream& db);



#endif