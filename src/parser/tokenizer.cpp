#include "tokenizer.h"

#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> tokenize(const std::string& input) {
    TokenizeResult result = tokenizeWithTypes(input);
    std::vector<std::string> texts;

    if (!result.success) {
        return {};
    }

    for (const Token& token : result.tokens) {
        texts.push_back(token.text);
    }

    return texts;
}



TokenizeResult tokenizeWithTypes(const std::string& input) {
    std::vector<Token> tokens;
    std::string currentToken;
    bool insideDoubleQuotes = false;
    bool insideSingleQuotes = false;
    bool tokenStarted = false;

    for (std::size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];

        if (ch == '\'' && !insideDoubleQuotes) {
            insideSingleQuotes = !insideSingleQuotes;
            tokenStarted = true;
            continue;
        }

        if (ch == '"' && !insideSingleQuotes) {
            insideDoubleQuotes = !insideDoubleQuotes;
            tokenStarted = true;
            continue;
        }

        if ((ch == ' ' || ch == '\t') &&
            !insideDoubleQuotes && !insideSingleQuotes) {

            if (tokenStarted) {
                tokens.push_back({currentToken, TokenType::Word});
                currentToken.clear();
                tokenStarted = false;
            }

        } else if ((ch == '|' || ch == '<' || ch == '>' || ch == '&') &&
                   !insideDoubleQuotes && !insideSingleQuotes) {

            if (tokenStarted) {
                tokens.push_back({currentToken, TokenType::Word});
                currentToken.clear();
                tokenStarted = false;
            }

            if (ch == '>' && i + 1 < input.size() && input[i + 1] == '>') {
                tokens.push_back({">>", TokenType::Operator});
                ++i;
            } else {
                tokens.push_back({std::string(1, ch), TokenType::Operator});
            }

        } else {
            currentToken += ch;
            tokenStarted = true;
        }
    }

        if (insideDoubleQuotes) {
    return {{}, false, "Error: Unclosed double quote."};
}

    if (insideSingleQuotes) {
    return {{}, false, "Error: Unclosed single quote."};
}

    if (tokenStarted) {
        tokens.push_back({currentToken, TokenType::Word});
    }

  return {tokens, true, ""};
}

