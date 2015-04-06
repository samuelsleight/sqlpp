#ifndef SQLLIB_TYPE_HPP
#define SQLLIB_TYPE_HPP

#include "defines.hpp"

#include <string>
#include <vector>

SQLLIB_NS

class Type {
public:
    virtual ~Type();

    // virtual static std::string sqlType() = 0;
};

SQLLIB_NS_END

#endif
