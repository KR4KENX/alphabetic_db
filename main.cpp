#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void bubbleSort(std::vector<string>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            //konkretny wyraz
            //iteracja po literach wyrazu
            //jesli wyraz[i][0] > wyraz[i+1][0] zamien je miejscami
            //jesli wyraz[i][0] = wyraz[i+1][0] petla
            // g = max(len(wyraz[i, wyraz[i+1))
            // jesli ktoras litera wyraz[i][g] > wyraz[i+1][g] zamien je miejscami
            // jesli wyrazy sa sobie rowne, mniejszy powinien byc wczesniej
            int g = 0;
            int first_word_size = arr[i].size();
            int second_Word_size = arr[i+1].size();
            bool shoul_swap_if_equal = false;

            if (first_word_size > second_Word_size) shoul_swap_if_equal = true;

            while(g < max(first_word_size, second_Word_size)){
                if ((g >= first_word_size || g >= second_Word_size) && shoul_swap_if_equal){

                }
            }
            if (arr[j][0] > arr[j + 1][0]) {
                string temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                swapped = true;
            }
        }

        if (!swapped) {
            break;
        }
    }
}
void write(fstream& db, vector<string>& words){
    string line;
    int line_counter = 0;
    while(getline(db, line)){
        cout << int(line[0]);
        line_counter++;
    }
}

int main(int argc, char* argv[]){
    fstream ou(argv[2]);
    // db for storing large amount of sentences
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
        bubbleSort(words);
        cout << endl;
        for(string word: words){
            cout << word << endl;
        }
        //write(ou, words);
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
    }
    ou.close();
    return 0;
}