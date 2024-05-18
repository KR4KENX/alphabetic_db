#include "utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


int main(int argc, char* argv[]){
    if(argc < 5){
        cout << "Operation not specified" << endl;
        return 1;
    }
    string userSecret = argv[3];
    if(argv[1][0] == 'm'){
        fstream db(argv[2], ios::out | ios::in | ios::binary);
        string tableNameToModify = argv[4];
        if (!db.good()){
            cout << "Error with opening db file" << endl;
            return 1;
        }
        cout << "Type in to add in table, add / before sentence if you want to delete from table" << endl;
        vector<string> wordsToAdd;
        vector<string> wordsToDrop;
        string word;
        while(getline(cin, word)){
            if(word[0] == '/') wordsToDrop.push_back(word.substr(1));
            else wordsToAdd.push_back(word);
        }
        modifyTable(db, userSecret, tableNameToModify, wordsToAdd, wordsToDrop);
    }
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
        dbTable newTable;
        newTable.secret = userSecret;
        newTable.tableName = argv[4];
        newTable.words = words;
        writeTableToDB(db, newTable);

        db.close();
    }
    if(argv[1][0] == 'r'){
        fstream db(argv[2], ios::in | ios::binary);
        string tableNameToRead = argv[4];
        // . for reading all tables
        if(!db.good()){
        cout << "Error with opening db file" << endl;
        return 1;
        }
        cout << "Read from database ... " << endl;
        vector<dbTable> userTables = readAllTablesFromDB(db, userSecret, tableNameToRead);
        if (userTables.size() > 0){
            for (dbTable table: userTables){
                cout << "---- " << table.tableName << " ----" << endl;
                for (string word: table.words){
                    cout << word << endl;
                }
            }
        }
    }
    return 0;
}