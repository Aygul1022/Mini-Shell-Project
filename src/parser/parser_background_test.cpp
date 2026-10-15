#include "parser.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    ParseResult single = parseCommandLine("sleep 10 &");

    if (single.success &&
        single.pipeline.commands.size() == 1 &&
        single.pipeline.commands[0].args ==
            std::vector<std::string>{"sleep", "10"} &&
        single.pipeline.background)
    {
        std::cout << "PASS: Single background command" << '\n';
    }
    else
    {
        std::cout << "FAIL: Single background command" << '\n';
        return 1;
    }

    ParseResult piped = parseCommandLine("ls | grep cpp &");

    if (piped.success &&
        piped.pipeline.commands.size() == 2 &&
        piped.pipeline.commands[0].args ==
            std::vector<std::string>{"ls"} &&
        piped.pipeline.commands[1].args ==
            std::vector<std::string>{"grep", "cpp"} &&
        piped.pipeline.background)
    {
        std::cout << "PASS: Background pipeline" << '\n';
    }
    else
    {
        std::cout << "FAIL: Background pipeline" << '\n';
        return 1;
    }

    std::vector<std::string> invalidInputs = {
        "&",
        "sleep 10 & ls",
        "ls | &"
    };

    for (const std::string &input : invalidInputs)
    {
        ParseResult invalid = parseCommandLine(input);

        if (!invalid.success && !invalid.error.empty())
        {
            std::cout << "PASS: Invalid background - "
                      << input << '\n';
        }
        else
        {
            std::cout << "FAIL: Invalid background - "
                      << input << '\n';
            return 1;
        }
    }

    return 0;
}