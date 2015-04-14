#ifndef SQLLIB_TYPE_HPP
#define SQLLIB_TYPE_HPP

#include "defines.hpp"
#include "statement.hpp"

#include <string>
#include <memory>

SQLLIB_NS

template<int N = 255>
class String {
public:
    using ValueType = std::string;

    String(ValueType value)
        : value(value) {}

    static std::string getSQLType() {
        return "VARCHAR(" + std::to_string(N) + std::string(")");
    }

    static ValueType getValue(std::shared_ptr<Statement> stmt, int index) {
        return stmt->stringValue(index);
    }

    void bind(int index, std::shared_ptr<Statement> stmt) {
        stmt->bindString(index, value);
    }

private:
    ValueType value;
};

class Integer {
public:
    using ValueType = int;

    Integer(ValueType value)
        : value(value) {}

    static std::string getSQLType() {
        return "INTEGER";
    }

    static ValueType getValue(std::shared_ptr<Statement> stmt, int index) {
        return stmt->intValue(index);
    }

    void bind(int index, std::shared_ptr<Statement> stmt) {
        stmt->bindInt(index, value);
    }

private:
    ValueType value;
};

SQLLIB_NS_END

#endif
