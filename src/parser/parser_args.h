#pragma once

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

#include "../exceptions.h"

class ParserArgs {
public:
    ParserArgs(size_t num_args, char** arguments);

    void SetFlag(std::string flag, size_t min_num_args);

    std::string GetFlag();

    std::string GetFirstArgument();

    std::vector<std::string> GetArgumentsFrom(size_t from);

private:
    size_t num_args_;
    std::vector<std::string> arguments_;
    std::unordered_map<std::string, size_t> min_num_args_;
};
