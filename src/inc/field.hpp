#ifndef SQLLIB_ROW_HPP
#define SQLLIB_ROW_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "db.hpp"

#include <tuple>
#include <type_traits>

SQLLIB_NS

template<typename T, int N>
class Field {
private:
    std::string name;
    static constexpr int fieldid = N;

public:
    using Type = T;

    Field(std::string name)
        : name(name) {}

    std::string getName() const {
        return name;
    }

    static constexpr int getN() {
        return fieldid;
    }
};

SQLLIB_NS_END

#endif
