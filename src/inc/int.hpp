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

    void bind(DB* db, int index) const {
        db->bindInteger(index, value);
    }
};

SQLLIB_NS_END

#endif

