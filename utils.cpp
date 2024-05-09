#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

bool biggerWord(string word1, string word2){
    for (char &c : word1)
        c = tolower(c);
    for (char &c : word2)
        c = tolower(c);
    // Word 1 > Word 2 = return True, Word 2 >= Word 1 = return False
    for(int i = 0; i < max(word1.size(), word2.size()); i++){
        if (i >= word1.size()) return false;
        if (i >= word2.size()) return true;
        if (word1[i] < word2[i]) return false;
        if (word1[i] > word2[i]) return true;
    }
    return false;
}
bool compareIgnoringCapital(char a, char b) {
    return std::tolower(a) < std::tolower(b);
}
bool linearSearchDB(ifstream& db, int r, string target){
    int l = 0;
    string line;
    while (l < r){
        getline(db, line);
        if (line == target) return true;
        if (biggerWord(line, target)) return false;
        l++;
    }
    return false;
}
int checkFileLenght(ifstream& db, int new_words_count){
    string line;
    int line_count;
    getline(db, line);
    line_count = stoi(line);
    return line_count + new_words_count;
}
void write(ifstream& db, ofstream& temp_db, vector<string>& words){
    int w_iterator = 0;
    string line;
    temp_db << to_string(checkFileLenght(db, words.size())) << '\n';
    while(getline(db, line)){
        if (line.empty()) break;
        if(w_iterator < words.size()){
            bool words_compare = biggerWord(line, words[w_iterator]);
            while(w_iterator < words.size() && words_compare){
                temp_db << words[w_iterator] << '\n';
                w_iterator++; 
                if (w_iterator >= words.size()) break;
                words_compare = biggerWord(line, words[w_iterator]);
            }
        }
        temp_db << line << '\n';
    }
    while(w_iterator < words.size()){
        temp_db << words[w_iterator] << '\n';
        w_iterator++;
    }
}
bool read(ifstream& db, string word){
    int line_count = checkFileLenght(db, 0);
    return linearSearchDB(db, line_count, word);
}