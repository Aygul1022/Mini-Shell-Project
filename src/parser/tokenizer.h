#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include <string>

enum class TokenType {
    Word,
    Operator
};

struct Token {
    std::string text;
    TokenType type;
};

struct TokenizeResult {
    std::vector<Token> tokens;
    bool success = true;
};


TokenizeResult tokenizeWithTypes(const std::string& input);
TokenizeResult tokenizeCommand(const std::string& input);

#endif