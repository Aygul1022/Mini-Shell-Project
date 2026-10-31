#include "parser.h"

#include <iostream>
#include <string>
#include <vector>

int main()
{
    ParseResult result = parseCommandLine("ls | grep cpp");

    if (result.success &&
        result.pipeline.commands.size() == 2 &&
        result.pipeline.commands[0].args.size() == 1 &&
        result.pipeline.commands[0].args[0] == "ls" &&
        result.pipeline.commands[1].args.size() == 2 &&
        result.pipeline.commands[1].args[0] == "grep" &&
        result.pipeline.commands[1].args[1] == "cpp" &&
        !result.pipeline.background)
    {
        std::cout << "PASS: Simple pipeline" << '\n';
    }
    else
    {
        std::cout << "FAIL: Simple pipeline" << '\n';
        return 1;
    }

    std::vector<std::string> invalidInputs = {
        "ls |",
        "| ls",
        "ls || grep cpp"};

    for (const std::string &input : invalidInputs)
    {
        ParseResult invalid = parseCommandLine(input);

        if (!invalid.success && !invalid.error.empty())
        {
            std::cout << "PASS: Invalid pipeline - "
                      << input << '\n';
        }
        else
        {
            std::cout << "FAIL: Invalid pipeline - "
                      << input << '\n';
            return 1;
        }
    }

    ParseResult threeStage = parseCommandLine(
        "cat file.txt | grep error | wc -l");

    if (threeStage.success &&
        threeStage.pipeline.commands.size() == 3 &&
        threeStage.pipeline.commands[0].args ==
            std::vector<std::string>{"cat", "file.txt"} &&
        threeStage.pipeline.commands[1].args ==
            std::vector<std::string>{"grep", "error"} &&
        threeStage.pipeline.commands[2].args ==
            std::vector<std::string>{"wc", "-l"} &&
        !threeStage.pipeline.background)
    {
        std::cout << "PASS: Three-stage pipeline" << '\n';
    }
    else
    {
        std::cout << "FAIL: Three-stage pipeline" << '\n';
        return 1;
    }

    ParseResult combined = parseCommandLine(
        "cat < input.txt | grep error > output.txt");

    if (combined.success &&
        combined.pipeline.commands.size() == 2 &&
        combined.pipeline.commands[0].args ==
            std::vector<std::string>{"cat"} &&
        combined.pipeline.commands[0].inputFile == "input.txt" &&
        combined.pipeline.commands[1].args ==
            std::vector<std::string>{"grep", "error"} &&
        combined.pipeline.commands[1].outputFile == "output.txt" &&
        !combined.pipeline.commands[1].appendOutput)
    {
        std::cout << "PASS: Pipeline with redirection" << '\n';
    }
    else
    {
        std::cout << "FAIL: Pipeline with redirection" << '\n';
        return 1;
    }

    return 0;
}