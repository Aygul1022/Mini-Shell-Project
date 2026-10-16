#include "parser.h"

#include <iostream>
#include <string>
#include <vector>

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
    }
    else
    {
        std::cout << "FAIL: Simple command" << '\n';
        return 1;
    }

    std::vector<std::string> inputs = {
        "echo \"|\"",
        "echo \">\"",
        "echo '&'"
    };

    std::vector<std::string> expectedWords = {
        "|",
        ">",
        "&"
    };

    for (std::size_t i = 0; i < inputs.size(); ++i)
    {
        ParseResult quoted = parseCommandLine(inputs[i]);

        if (quoted.success &&
            quoted.pipeline.commands.size() == 1 &&
            quoted.pipeline.commands[0].args ==
                std::vector<std::string>{"echo", expectedWords[i]} &&
            !quoted.pipeline.background)
        {
            std::cout << "PASS: Quoted operator - "
                      << expectedWords[i] << '\n';
        }
        else
        {
            std::cout << "FAIL: Quoted operator - "
                      << expectedWords[i] << '\n';
            return 1;
        }
    }

    return 0;
}