#ifndef SQLLIB_STRING_HPP
#define SQLLIB_STRING_HPP

#include "defines.hpp"
#include "type.hpp"

#include <string>
#include <sstream>

SQLLIB_NS

template<int N = 255>
class String : public Type {
private:
    std::string value;

public:
    String(std::string value) : value(value) {}

    static std::string sqlType() {
        std::ostringstream str;
        str << "VARCHAR(" << N << ")";
        return str.str();
    }
};

SQLLIB_NS_END

#endif
