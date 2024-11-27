#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <string>

class DatabaseException : public std::exception {
private:
    std::string message;

public:
    explicit DatabaseException(const std::string &msg) : message(msg) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

#endif // DATABASEEXCEPTION_H
