#include "tokenizer.h"

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string currentToken;
    bool insideSingleQuotes = false;
    bool insideQuotes = false;
    bool tokenStarted = false;

    for (std::size_t i = 0; i < input.size(); ++i) {
        char ch = input[i];
        if (ch == '\'' && !insideQuotes) {
    insideSingleQuotes = !insideSingleQuotes;
    tokenStarted = true;
    continue;
}

        if (ch == '"' && !insideSingleQuotes) {
    insideQuotes = !insideQuotes;
    tokenStarted = true;
    continue;
}

        if ((ch == ' ' || ch == '\t') && !insideQuotes && !insideSingleQuotes) {
            if (!currentToken.empty() || tokenStarted) {
                tokens.push_back(currentToken);
                currentToken.clear();
                tokenStarted = false;
            }
        } else if (!insideQuotes && !insideSingleQuotes && (ch == '|' || ch == '>' || ch == '<' || ch == '&')) {
           if (ch == '>' && i + 1 < input.size() && input[i + 1] == '>') {
    if (!currentToken.empty() || tokenStarted) {
    tokens.push_back(currentToken);
    currentToken.clear();
    tokenStarted = false;
}
tokens.push_back(">>");
    ++i;
    continue;
}
    if (!currentToken.empty() || tokenStarted) {
    tokens.push_back(currentToken);
    currentToken.clear();
    tokenStarted = false;
}
tokens.push_back(std::string(1, ch));
} else {
    currentToken += ch;
    tokenStarted = true;
}
    }

    if (insideQuotes) {
    std::cerr << "Error: Unclosed double quote." << '\n';
    return {};
}

    if (insideSingleQuotes) {
    std::cerr << "Error: Unclosed single quote." << '\n';
    return {};
}

    if (!currentToken.empty() || tokenStarted) {
    tokens.push_back(currentToken);
}

return tokens;
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
        std::cerr << "Error: Unclosed double quote." << '\n';
        return {{}, false};
    }

    if (insideSingleQuotes) {
    std::cerr << "Error: Unclosed single quote." << '\n';
    return {{}, false};
}

    if (tokenStarted) {
        tokens.push_back({currentToken, TokenType::Word});
    }

    return {tokens, true};
}




TokenizeResult tokenizeCommand(const std::string& input) {
    return tokenizeWithTypes(input);
}