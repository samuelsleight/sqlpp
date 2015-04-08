#ifndef SQLLIB_INT_HPP
#define SQLLIB_INT_HPP

#include "defines.hpp"
#include "type.hpp"

SQLLIB_NS

class Integer : public Type {
private:
    int val;

public:
    Integer() {}
    Integer(int val) : val(val) {}

    static std::string sqlType() {
        return "INTEGER";
    }

    static Integer getValue(DB* db, int index) {
        return Integer(db->selectIntegerValue(index));
    }

    int value() {
        return val;
    }

    void bind(DB* db, int index) const {
        db->bindInteger(index, val);
    }
};

SQLLIB_NS_END

#endif

