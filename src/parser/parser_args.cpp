#include "parser_args.h"

ParserArgs::ParserArgs(size_t num_args, char** arguments) : num_args_(num_args) {
    for (size_t i = 0; i < num_args; ++i) {
        arguments_.push_back(arguments[i]);
    }
}

std::string ParserArgs::GetFlag() {
    if (num_args_ == 1) {
        throw NoFlags();
    }
    if (!min_num_args_.contains(arguments_[1])) {
        throw IncorrectFlagName(arguments_[1]);
    }
    return arguments_[1];
}

std::string ParserArgs::GetFirstArgument() {
    if (num_args_ < 3 || min_num_args_[arguments_[1]] < 1) {
        throw NoFewArguments();
    }
    return arguments_[2];
}

void ParserArgs::SetFlag(std::string flag, size_t min_num_args) {
    min_num_args_[flag] = min_num_args;
}

std::vector<std::string> ParserArgs::GetArgumentsFrom(size_t from) {
    if (num_args_ <= from + 1) {
        throw NoFewArguments();
    }
    if (min_num_args_[arguments_[1]] < from) {
        throw IncorrectFlagName(arguments_[1]);
    }
    std::vector<std::string> args;
    for (size_t i = from + 1; i < num_args_; ++i) {
        args.push_back(arguments_[i]);
    }
    return args;
}
