#include <iostream>
#include "CommandHandler.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "The count of arguments must be 2" << endl;
        return 1;
    }
    
    GameSystem system(argv[1], argv[2]);
    CommandHandler handler(&system);
    string command;

    while (getline(cin, command)) {
        handler.handle(command);
    }
}