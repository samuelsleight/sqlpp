#ifndef SQLLIB_TYPE_HPP
#define SQLLIB_TYPE_HPP

#include "defines.hpp"

#include <string>
#include <vector>

SQLLIB_NS

class DB;

class Type {
public:
    virtual ~Type() {}

    virtual void bind(DB* db, int index) const = 0;
};

SQLLIB_NS_END

#endif
