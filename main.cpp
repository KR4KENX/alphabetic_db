#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
bool biggerWord(string word1, string word2){
    // Word 1 > Word 2 = return True, Word 2 >= Word 1 = return False
    for(int i = 0; i < max(word1.size(), word2.size()); i++){
        if (i >= word1.size()) return false;
        if (i >= word2.size()) return true;
        if (word1[i] < word2[i]) return false;
        if (word1[i] > word2[i]) return true;
    }
    return false;
}
void write(ifstream& db, ofstream& temp_db, vector<string>& words){
    db.clear();
    db.seekg(0, ios::beg);
    int w_iterator = 0;
    string line;
    //TODO looping through words vector
    while(getline(db, line)){
        if (w_iterator >= words.size()) break;
        bool words_compare = biggerWord(words[w_iterator], line);
   /*     if (words_compare){
            string existing_text = line; // Zachowanie istniejącego tekstu
            line = words[w_iterator]; // Dopisanie nowego tekstu
            cout << existing_text << " : " << line;
            db.seekp(db.tellg()); // Ustawienie wskaźnika zapisu na końcu aktualnej linii
            db << line; // Zapisanie nowej linii
            db << existing_text; // Dopisanie zachowanego istniejącego tekstu


            w_iterator++;
        } */
        if (line == "abecadlo"){
            // string temp = line;
            // db.seekp(-line.length()-1, ios::cur);
            // db << words[0] << '\n';
            temp_db << words[0] << '\n';
            temp_db << "abecadlo" << '\n';
        } 
        else{
            temp_db << line << '\n';
        } 
    }
}

int main(int argc, char* argv[]){
    ofstream in("dbdata/temp");
    ifstream ou(argv[2]);
    if(argc < 3){
        cout << "Operation not specified" << endl;
        return 1;
    }
    if(!ou.good()){
        cout << "Error with opening db file" << endl;
        return 1;
    }
    if(argv[1][0] == 'w'){
        cout << "Write to database, add ENTER afer every sentence ... " << endl;
        vector<string> words;
        string word;
        while(getline(cin, word)){
            words.push_back(word);
        }
        sort(words.begin(), words.end());
        write(ou, in, words);
        remove(argv[2]);
        rename("dbdata/temp", argv[2]);
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
    }
    ou.close();
    in.close();
    return 0;
}