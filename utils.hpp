#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool compareIgnoringCapital(char a, char b);
void write(fstream& db, vector<string>& words);
vector<string> read(fstream& db);
void sortWordsV(vector<string>& words);



#endif