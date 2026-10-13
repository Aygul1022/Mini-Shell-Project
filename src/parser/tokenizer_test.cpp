#include "tokenizer.h"

#include <iostream>
#include <string>
#include <vector>


int main() {
std::vector<std::string> testInputs = {
    "echo hello",
    "echo \"hello world\"",
    "echo hello>output.txt",
    "echo \"\"",
    "ls | grep cpp",
    "ls|grep cpp",
    "echo hello>>output.txt",
    "cat<input.txt",
    "sleep 5 &",
    "sleep 5&",
    "echo \"hello|world\"",
    "echo 'hello>world'",
    "echo \"hello\" 'world'",
    "echo a\"\"b",
    "echo ''"
};

std::vector<std::vector<std::string>> expectedTokens = {
    {"echo", "hello"},
    {"echo", "hello world"},
    {"echo", "hello", ">", "output.txt"},
    {"echo", ""},
    {"ls", "|", "grep", "cpp"},
    {"ls", "|", "grep", "cpp"},
    {"echo", "hello", ">>", "output.txt"},
    {"cat", "<", "input.txt"},
    {"sleep", "5", "&"},
    {"sleep", "5", "&"},
    {"echo", "hello|world"},
    {"echo", "hello>world"},
    {"echo", "hello", "world"},
    {"echo", "ab"},
    {"echo", ""}
};

for (std::size_t i = 0; i < testInputs.size(); ++i) {
    const std::string& input = testInputs[i];
    std::cout << "Input: " << input << '\n';
    std::vector<std::string> tokens = tokenize(input);

    if (tokens == expectedTokens[i]) {
        std::cout << "PASS" << '\n';
    } else {
        std::cout << "FAIL" << '\n';
    }

    for (const std::string& token : tokens) {
        std::cout << "[" << token << "]" << '\n';
    }

    std::cout << '\n';
}

return 0;
}
