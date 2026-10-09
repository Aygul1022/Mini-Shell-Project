#include <iostream>
#include <string>

using namespace std;

int main(){
    string input;

    while (true){
        cout << "mini-shell> ";
        getline(cin, input);

        if(cin.eof()){
            break;
        }
        if(input.find_first_not_of("\t\r\n") == string::npos){
            continue;
        }

        if(input == "exit"){
            break;
        }
        else {
            cout << "You entered: " << input << endl;

        }
    }

    return 0;
}