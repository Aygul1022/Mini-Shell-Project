#include <iostream>
#include <string>
#include "parser/parser.h"
#include "builtins/builtins.h"

using namespace std;

// Return true only for a supported exit request. Reject unsupported execution
// forms before any built-in can change the shell's state.
bool dispatchPipeline(const Pipeline& pipeline){
    if(pipeline.commands.empty()){
        return false;
    }
    if(pipeline.background){
        cerr << "mini-shell: background execution awaits executor integration" << endl;
        return false;
    }
    if(pipeline.commands.size() > 1){
        cerr << "mini-shell: pipelines await executor integration" << endl;
        return false;
    }

    const Command& command = pipeline.commands.front();
    if(!command.inputFile.empty() || !command.outputFile.empty()){
        cerr << "mini-shell: redirection awaits executor integration" << endl;
        return false;
    }
    if(command.args.empty()){
        return false;
    }
    const string& name = command.args[0];
    if(name.empty()){
        cerr << "mini-shell: empty command name" << endl;
        return false;
    }
    const BuiltinResult result = dispatchBuiltin(command.args);
    if(result == BuiltinResult::ExitRequested){
        return true;
    }
    if(result == BuiltinResult::Handled){
        return false;
    }
    cerr << "mini-shell: external command execution awaits executor integration: "
         << name << endl;
    return false;
}

int main(){
    string input;

    while (true){
        cout << "mini-shell> ";
        if(!getline(cin, input)){
            if(cin.bad() || !cin.eof()){
                cerr << "mini-shell: failed to read input" << endl;
                return 1;
            }
            break;
        }
        if(input.find_first_not_of(" \t\r\n\v\f") == string::npos){
            continue;
        }

        const ParseResult result = parseCommandLine(input);
        if(!result.success){
            cerr << "mini-shell: " << result.error << endl;
            continue;
        }
        if(dispatchPipeline(result.pipeline)){
            break;
        }
    }

    return 0;
}
