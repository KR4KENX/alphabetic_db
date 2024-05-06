#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    if(argc < 2){
        cout << "Operation not specified" << endl;
        return 1;
    }
    if(argv[1][0] == 'w'){
        cout << "Write to database ... " << endl;
    }
    if(argv[1][0] == 'r'){
        cout << "Read from database ... " << endl;
    }
    return 0;
}