#include "exception.hpp"

SQLLIB_NS

Exception::Exception(std::string message)
    : message(message) {}

const char* Exception::what() const noexcept {
    return message.c_str();
}

SQLLIB_NS_END
