#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


int main(int argc, char* argv[]){
    if(argc < 4){
        cout << "Operation not specified" << endl;
        return 1;
    }
    string userSecret = argv[3];
    if(argv[1][0] == 'c'){
        fstream db(argv[2], ios::app | ios::binary);
        if(!db.good()){
        cout << "Error with opening db file" << endl;
        return 1;
        }
        cout << "Create DB table, add words in your table - ENTER afer every sentence ... " << endl;
        vector<string> words;
        string word;
        while(getline(cin, word)){
            words.push_back(word);
        }
        writeTableToDB(db, words, userSecret);

        db.close();
    }
    if(argv[1][0] == 'r'){
        fstream db(argv[2], ios::in | ios::binary);
        if(!db.good()){
        cout << "Error with opening db file" << endl;
        return 1;
        }
        cout << "Read from database ... " << endl;
        vector<dbTable> allTables = readAllTablesFromDB(db);
        db.close();

        cout << allTables[1].words[1] << endl;
    }
    return 0;
}