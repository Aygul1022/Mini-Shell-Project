#include "parser.h"

#include <iostream>

int main()
{
    ParseResult result = parseCommandLine("echo hello");

    if (result.success &&
        result.pipeline.commands.size() == 1 &&
        result.pipeline.commands[0].args.size() == 2 &&
        result.pipeline.commands[0].args[0] == "echo" &&
        result.pipeline.commands[0].args[1] == "hello" &&
        !result.pipeline.background)
    {
        std::cout << "PASS: Simple command" << '\n';
        return 0;
    }

    std::cout << "FAIL: Simple command" << '\n';
    return 1;
}