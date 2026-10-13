#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>


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
    std::string error;
};

std::vector<std::string> tokenize(const std::string& input);
TokenizeResult tokenizeWithTypes(const std::string& input);

#endif