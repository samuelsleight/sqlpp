#ifndef SQLLIB_INT_HPP
#define SQLLIB_INT_HPP

#include "defines.hpp"
#include "type.hpp"

SQLLIB_NS

class Integer : public Type {
private:
    int value;

public:
    Integer(int value) : value(value) {}

    static std::string sqlType() {
        return "INTEGER";
    }
};

SQLLIB_NS_END

#endif

