#ifndef SQLLIB_EXCEPTION_HPP
#define SQLLIB_EXCEPTION_HPP

#include "defines.hpp"

#include <string>
#include <exception>

SQLLIB_NS

class Exception : public std::exception {
public:
    Exception(std::string message);

    const char* what() const noexcept override;

private:
    std::string message;
};

SQLLIB_NS_END

#endif
