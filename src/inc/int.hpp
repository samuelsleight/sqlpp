#ifndef SQLLIB_INT_HPP
#define SQLLIB_INT_HPP

#include "defines.hpp"
#include "type.hpp"

SQLLIB_NS

class Integer : public Type {
private:
    int value;

public:
    Integer() {}
    Integer(int value) : value(value) {}

    static std::string sqlType() {
        return "INTEGER";
    }

    static int getValue(DB* db, int index) {
        return db->selectIntegerValue(index);
    }

    void bind(DB* db, int index) const {
        db->bindInteger(index, value);
    }
};

SQLLIB_NS_END

#endif

