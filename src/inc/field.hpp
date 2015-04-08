#ifndef SQLLIB_ROW_HPP
#define SQLLIB_ROW_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "db.hpp"

#include <tuple>
#include <type_traits>

SQLLIB_NS

template<typename T, int N, int TID>
class Field {
private:
    std::string tableName, name;
    static constexpr int fieldid = N;
    static constexpr int tableid = TID;

public:
    using Type = T;

    Field(std::string tableName, std::string name)
        : tableName(tableName), name(name) {}

    std::string getName() const {
        return name;
    }

    std::string getTableName() const {
        return tableName;
    }

    static constexpr int getN() {
        return fieldid;
    }

    static constexpr int getTID() {
        return tableid;
    }
};

SQLLIB_NS_END

#endif
