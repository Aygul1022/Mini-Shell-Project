#include "parser.h"

#include <iostream>

int main()
{
    ParseResult valid = parseCommandLine("echo hello > output.txt");

    if (valid.success &&
        valid.pipeline.commands.size() == 1 &&
        valid.pipeline.commands[0].args.size() == 2 &&
        valid.pipeline.commands[0].args[0] == "echo" &&
        valid.pipeline.commands[0].args[1] == "hello" &&
        valid.pipeline.commands[0].outputFile == "output.txt" &&
        !valid.pipeline.commands[0].appendOutput)
    {
        std::cout << "PASS: Output redirection" << '\n';
    }
    else
    {
        std::cout << "FAIL: Output redirection" << '\n';
        return 1;
    }

    ParseResult invalid = parseCommandLine("echo hello >");

    if (!invalid.success &&
        !invalid.error.empty())
    {
        std::cout << "PASS: Missing filename" << '\n';
    }
    else
    {
        std::cout << "FAIL: Missing filename" << '\n';
        return 1;
    }

    ParseResult inputRedirect = parseCommandLine("cat < input.txt");

    if (inputRedirect.success &&
        inputRedirect.pipeline.commands.size() == 1 &&
        inputRedirect.pipeline.commands[0].args.size() == 1 &&
        inputRedirect.pipeline.commands[0].args[0] == "cat" &&
        inputRedirect.pipeline.commands[0].inputFile == "input.txt")
    {
        std::cout << "PASS: Input redirection" << '\n';
    }
    else
    {
        std::cout << "FAIL: Input redirection" << '\n';
        return 1;
    }
    ParseResult appendRedirect = parseCommandLine("echo hello >> output.txt");

    if (appendRedirect.success &&
        appendRedirect.pipeline.commands.size() == 1 &&
        appendRedirect.pipeline.commands[0].args.size() == 2 &&
        appendRedirect.pipeline.commands[0].args[0] == "echo" &&
        appendRedirect.pipeline.commands[0].args[1] == "hello" &&
        appendRedirect.pipeline.commands[0].outputFile == "output.txt" &&
        appendRedirect.pipeline.commands[0].appendOutput)
    {
        std::cout << "PASS: Append redirection" << '\n';
    }
    else
    {
        std::cout << "FAIL: Append redirection" << '\n';
        return 1;
    }
    return 0;
}