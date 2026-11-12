#ifndef PARSER_H
#define PARSER_H

#include "../command.h"

#include <string>

struct ParseResult
{
    Pipeline pipeline;
    bool success = true;
    std::string error;
};

ParseResult parseCommandLine(const std::string &input);

#endif