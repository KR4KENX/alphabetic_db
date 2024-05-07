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
    int w_iterator = 0;
    string line;
    while(getline(db, line)){
        if (line.empty()) break;
        if(w_iterator < words.size()){
            bool words_compare = biggerWord(line, words[w_iterator]);
            cout << words_compare << " : " << line << " : " << words[w_iterator] << endl;
            while(w_iterator < words.size() && words_compare){
                cout << "Inner loop";
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

int main(int argc, char* argv[]){
    ofstream ou("dbdata/temp");
    ifstream in(argv[2]);
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
        write(in, ou, words);
        remove(argv[2]);
        rename("dbdata/temp", argv[2]);
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
    }
    in.close();
    ou.close();
    return 0;
}