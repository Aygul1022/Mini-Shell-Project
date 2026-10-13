#include "tokenizer.h"

#include <iostream>
#include <vector>

int main()
{

    TokenizeResult firstResult = tokenizeWithTypes("ls|grep cpp");
    std::vector<Token> tokens = firstResult.tokens;

    if (firstResult.success &&
        tokens.size() == 4 &&
        tokens[0].text == "ls" &&
        tokens[0].type == TokenType::Word &&
        tokens[1].text == "|" &&
        tokens[1].type == TokenType::Operator &&
        tokens[2].text == "grep" &&
        tokens[2].type == TokenType::Word &&
        tokens[3].text == "cpp" &&
        tokens[3].type == TokenType::Word)
    {
        std::cout << "PASS" << '\n';
    }
    else
    {
        std::cout << "FAIL" << '\n';
    }

    std::vector<Token> quotedTokens = tokenizeWithTypes("echo \"|\"").tokens;

    if (quotedTokens.size() == 2 &&
        quotedTokens[0].text == "echo" &&
        quotedTokens[0].type == TokenType::Word &&
        quotedTokens[1].text == "|" &&
        quotedTokens[1].type == TokenType::Word)
    {
        std::cout << "PASS: Quoted pipe" << '\n';
    }
    else
    {
        std::cout << "FAIL: Quoted pipe" << '\n';
    }

    std::vector<std::string> singleQuoteInputs = {
        "echo 'hello world'",
        "echo 'hello|world'",
        "echo 'hello\"world'"};

    std::vector<std::string> expectedWords = {
        "hello world",
        "hello|world",
        "hello\"world"};

    for (std::size_t i = 0; i < singleQuoteInputs.size(); ++i)
    {
        std::vector<Token> result = tokenizeWithTypes(singleQuoteInputs[i]).tokens;

        if (result.size() == 2 &&
            result[0].text == "echo" &&
            result[0].type == TokenType::Word &&
            result[1].text == expectedWords[i] &&
            result[1].type == TokenType::Word)
        {
            std::cout << "PASS: Single quote test " << i + 1 << '\n';
        }
        else
        {
            std::cout << "FAIL: Single quote test " << i + 1 << '\n';
        }
    }

    std::vector<std::string> operatorInputs = {
        "cat<input.txt",
        "echo hello>output.txt",
        "echo hello>>output.txt",
        "sleep 5&"};

    std::vector<std::string> expectedOperators = {
        "<",
        ">",
        ">>",
        "&"};

    for (std::size_t i = 0; i < operatorInputs.size(); ++i)
    {
        std::vector<Token> result = tokenizeWithTypes(operatorInputs[i]).tokens;

        bool foundOperator = false;

        for (const Token &token : result)
        {
            if (token.text == expectedOperators[i] &&
                token.type == TokenType::Operator)
            {
                foundOperator = true;
            }
        }

        if (foundOperator)
        {
            std::cout << "PASS: Operator test " << i + 1 << '\n';
        }
        else
        {
            std::cout << "FAIL: Operator test " << i + 1 << '\n';
        }
    }

    std::vector<std::string> unclosedQuoteInputs = {
        "echo \"hello",
        "echo 'hello"};

    for (std::size_t i = 0; i < unclosedQuoteInputs.size(); ++i)
    {
        TokenizeResult result = tokenizeWithTypes(unclosedQuoteInputs[i]);

        if (!result.success &&
            result.tokens.empty() &&
            result.error == (i == 0
                                 ? "Unclosed double quote."
                                 : "Unclosed single quote."))
        {
            std::cout << "PASS: Unclosed quote test " << i + 1 << '\n';
        }
        else
        {
            std::cout << "FAIL: Unclosed quote test " << i + 1 << '\n';
        }
    }

    std::vector<Token> emptyQuoteTokens = tokenizeWithTypes("echo \"\"").tokens;

    if (emptyQuoteTokens.size() == 2 &&
        emptyQuoteTokens[0].text == "echo" &&
        emptyQuoteTokens[0].type == TokenType::Word &&
        emptyQuoteTokens[1].text == "" &&
        emptyQuoteTokens[1].type == TokenType::Word)
    {
        std::cout << "PASS: Empty quote" << '\n';
    }
    else
    {
        std::cout << "FAIL: Empty quote" << '\n';
    }

    std::vector<std::string> emptyInputs = {
        "",
        "   "};

    for (std::size_t i = 0; i < emptyInputs.size(); ++i)
    {
        TokenizeResult result = tokenizeWithTypes(emptyInputs[i]);

        if (result.success && result.tokens.empty())
        {
            std::cout << "PASS: Empty input test " << i + 1 << '\n';
        }
        else
        {
            std::cout << "FAIL: Empty input test " << i + 1 << '\n';
        }
    }

    return 0;
}
