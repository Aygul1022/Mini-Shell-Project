#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

struct Command {
    std::vector<std::string> args;

    std::string inputFile;
    std::string outputFile;

    bool appendOutput = false;
};
struct Pipeline {
    std::vector<Command> commands;
};
#endif
