#ifndef SQLLIB_SELECT_HPP
#define SQLLIB_SELECT_HPP

#include "defines.hpp"

#include <tuple>
#include <memory>

SQLLIB_NS

class DB;

template<typename, typename...>
class Table;

template<typename FieldTuple, typename... FieldTypes>
class Select : public std::enable_shared_from_this<Select<FieldTuple, FieldTypes...>> {
public:
    using Ptr = std::shared_ptr<Select<FieldTuple, FieldTypes...>>;

private:
    template<typename, typename...>
    friend class Table;

    DB* db;
    std::string table;
    FieldTuple rows;

    Select(DB* db, std::string table, FieldTuple rows)
        : db(db), table(table), rows(rows) {}
};

SQLLIB_NS_END

#endif
