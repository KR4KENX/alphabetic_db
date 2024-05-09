#ifndef UTILS_HPP_
#define UTILS_HPP_
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool biggerWord(string word1, string word2);
bool compareIgnoringCapital(char a, char b);
bool linearSearchDB(ifstream& db, int r, string target);
int checkFileLenght(ifstream& db, int new_words_count);
void write(ifstream& db, ofstream& temp_db, vector<string>& words);
bool read(ifstream& db, string word);



#endif