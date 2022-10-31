#pragma once

#include <exception>
#include <string>

class IncorrectArchive : public std::exception {
public:
    explicit IncorrectArchive(const std::string &archive_name)
        : error_description_("Archive " + archive_name + " is incorrect\n") {
    }

    const char *what() const noexcept override {
        return error_description_.c_str();
    }

private:
    std::string error_description_;
};

class IncorrectFile : public std::exception {
public:
    explicit IncorrectFile(const std::string &filename) : error_description_("File " + filename + " is incorrect\n") {
    }

    const char *what() const noexcept override {
        return error_description_.c_str();
    }

private:
    std::string error_description_;
};

class NoFlags : public std::exception {
public:
    explicit NoFlags() : error_description_("There are no flags\nPlease use '-h' to see manual\n") {
    }

    const char *what() const noexcept override {
        return error_description_.c_str();
    }

private:
    std::string error_description_;
};

class IncorrectFlagName : public std::exception {
public:
    explicit IncorrectFlagName(const std::string &user_flag_name)
        : error_description_("Flag " + user_flag_name + " does not exists\nPlease use '-h' to see manual\n") {
    }

    const char *what() const noexcept override {
        return error_description_.c_str();
    }

private:
    std::string error_description_;
};

class NoFewArguments : public std::exception {
public:
    explicit NoFewArguments() : error_description_("Arguments are not correct.\nPlease use '-h' to see manual\n") {
    }

    const char *what() const noexcept override {
        return error_description_.c_str();
    }

private:
    std::string error_description_;
};
