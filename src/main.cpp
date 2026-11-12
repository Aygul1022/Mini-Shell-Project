#include <iostream>
#include <string>
#include "parser/parser.h"
#include "builtins/builtins.h"
#include "process/executor.h"
#include "process/pipeline_executor.h"

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
        // Validate every stage before starting any process.
        for(const Command& stage : pipeline.commands){
            if(stage.args.empty() || stage.args[0].empty()){
                cerr << "mini-shell: empty command name in pipeline" << endl;
                return false;
            }
            if(isBuiltin(stage.args[0])){
                cerr << "mini-shell: built-ins in pipelines are not supported yet" << endl;
                return false;
            }
            if(!stage.inputFile.empty() || !stage.outputFile.empty()){
                cerr << "mini-shell: redirection within pipelines awaits executor integration" << endl;
                return false;
            }
        }
        cout.flush();
        executePipeline(pipeline);
        return false;
    }

    const Command& command = pipeline.commands.front();
    if(command.args.empty()){
        return false;
    }
    const string& name = command.args[0];
    if(name.empty()){
        cerr << "mini-shell: empty command name" << endl;
        return false;
    }
    if(isBuiltin(name) && (!command.inputFile.empty() || !command.outputFile.empty())){
        cerr << "mini-shell: redirection for built-ins is not supported yet" << endl;
        return false;
    }
    const BuiltinResult result = dispatchBuiltin(command.args);
    if(result == BuiltinResult::ExitRequested){
        return true;
    }
    if(result == BuiltinResult::Handled){
        return false;
    }
    cout.flush();
    executeCommand(command);
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
