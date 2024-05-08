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
        sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
            return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), compareIgnoringCapital);
        });
        write(in, ou, words);
        remove(argv[2]);
        rename("dbdata/temp", argv[2]);
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
        string word;
        getline(cin, word);
        if (read(in, word)) cout << "Word already in DB" << endl;
        else cout << "Word not in DB" << endl;
    }
    in.close();
    ou.close();
    return 0;
}