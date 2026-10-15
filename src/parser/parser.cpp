#include "parser.h"
#include "tokenizer.h"

ParseResult parseCommandLine(const std::string &input)
{
    TokenizeResult tokenResult = tokenizeWithTypes(input);

    if (!tokenResult.success)
    {
        return {{}, false, tokenResult.error};
    }

    ParseResult result;

    if (tokenResult.tokens.empty())
    {
        return result;
    }

    Command command;

    for (std::size_t i = 0; i < tokenResult.tokens.size(); ++i)
    {
        const Token &token = tokenResult.tokens[i];

        if (token.type == TokenType::Word)
        {
            command.args.push_back(token.text);
            continue;
        }

        if (token.text == "<" ||
            token.text == ">" ||
            token.text == ">>")
        {
            if (i + 1 >= tokenResult.tokens.size() ||
                tokenResult.tokens[i + 1].type != TokenType::Word)
            {
                return {{}, false, "Missing filename after redirection operator."};
            }

            const std::string &filename = tokenResult.tokens[i + 1].text;

            if (token.text == "<")
            {
                command.inputFile = filename;
            }
            else
            {
                command.outputFile = filename;
                command.appendOutput = (token.text == ">>");
            }

            ++i;
            continue;
        }

        return {{}, false, "Operator parsing is not implemented yet."};
    }

    if (command.args.empty())
    {
        return {{}, false, "Missing command."};
    }

    result.pipeline.commands.push_back(command);

    return result;
}