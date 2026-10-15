#include "builtins.h"

#include <iostream>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace std;

namespace {
// Arguments include the command name and come from the shared parser.
// Directory changes must run in the shell process.
int changeDirectory(const vector<string>& arguments){
    if(arguments.size() > 2){
        cerr << "mini-shell: cd: too many arguments" << endl;
        return 1;
    }

    const char* path = arguments.size() == 2
        ? arguments[1].c_str() : getenv("HOME");
    
  
    // ıf size of string vector arguments is 1, then path is set to the value of the HOME environment variable. 
    // If HOME is not set, getenv() returns nullptr. In that case, print an error message and return 1.
    
    if(path == nullptr){
        cerr << "mini-shell: cd: HOME is not set" << endl;
        return 1;
    }

    // chdir() returns -1 on error, sets errno, and does not print an error message. its -1 because it returns the value of the system call, which is -1 on failure. The perror() function prints a descriptive error message to stderr based on the current value of errno.
    if(chdir(path) == -1){
        perror("mini-shell: cd");
        return 1;
    }
    return 0;
}

void printWorkingDirectory(){
    vector<char> buffer(256);

    while(getcwd(buffer.data(), buffer.size()) == nullptr){
        if(errno != ERANGE){
            perror("mini-shell: pwd");
            return;
        }
        // Retry with more space when the directory path does not fit.
        buffer.resize(buffer.size() * 2);
    }

    cout << buffer.data() << endl;
}

void printHelp(){
    cout << "Mini Shell - available commands\n"
         << "  help       Display this help message\n"
         << "  pwd        Print the current working directory\n"
         << "  cd [path]  Change directory; no path uses HOME\n"
         << "  exit       Exit the shell\n"
         << "  Ctrl+D     Exit on an empty input line\n"
         << "\nExamples: cd /tmp, cd .., cd\n"
         << "\nCurrent limitations:\n"
         << "  Spaces and tabs around commands are accepted.\n"
         << "  Quote paths containing spaces: cd \"My Folder\"\n"
         << "  help, pwd, and exit accept no arguments.\n"
         << "  Variable expansion, ~ expansion, and escaping are unavailable.\n"
         << "  External commands, pipelines, redirection, and background execution\n"
         << "  await executor integration and are rejected for now.\n";
}

} // namespace

BuiltinResult dispatchBuiltin(const vector<string>& args){
    if(args.empty()){
        return BuiltinResult::NotBuiltin;
    }
    const string& name = args[0];
    if(name == "cd"){
        changeDirectory(args);
        return BuiltinResult::Handled;
    }
    if(name == "pwd" || name == "help" || name == "exit"){
        if(args.size() != 1){
            cerr << "mini-shell: " << name << ": arguments are not supported" << endl;
            return BuiltinResult::Handled;
        }
        if(name == "exit"){
            return BuiltinResult::ExitRequested;
        }
        if(name == "pwd"){
            printWorkingDirectory();
        }
        else {
            printHelp();
        }
        return BuiltinResult::Handled;
    }
    return BuiltinResult::NotBuiltin;
}
