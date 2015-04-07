#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "type.hpp"
#include "row.hpp"
#include "insert.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <memory>
#include <type_traits>

SQLLIB_NS

class DB;

template<typename RowTuple, typename... RowTypes>
class Table : public std::enable_shared_from_this<Table<RowTuple, RowTypes...>> {
public:
    using Ptr = std::shared_ptr<Table<RowTuple, RowTypes...>>;

    template<typename T, int N>
    auto addRow(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");
        static_assert(!RowTupleIDExists<RowTuple, N>::value, "row identifier must be unique");

        auto newTuple = std::tuple_cat(rows, std::make_tuple(Row<T, N>(rowName)));

        using NewTuple = decltype(newTuple);
        using NewTable = Table<NewTuple, RowTypes..., T>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, newTuple));
    }

    const std::string getName() const {
        return name;
    }

    template<int N>
    auto getRow() {
        static_assert(RowTupleIDExists<RowTuple, N>::value, "row identifier does not exist");
        return RowTupleGet<RowTuple, N>::get(rows);
    }

    typename Insert<RowTuple, RowTypes...>::Ptr insert() {
        return typename Insert<RowTuple, RowTypes...>::Ptr(new Insert<RowTuple, RowTypes...>(db, name, rows));
    }

    Ptr create();

private:
    friend class DB;

    template<typename, typename...>
    friend class Table;

    DB* db;
    std::string name;
    RowTuple rows;

    Table(DB* db, std::string name, RowTuple rows = std::tuple<>())
        : db(db), name(name), rows(rows) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename RowTuple, typename... RowTypes>
typename Table<RowTuple, RowTypes...>::Ptr Table<RowTuple, RowTypes...>::create() {
    std::ostringstream str;
    str << "CREATE TABLE " << name << "(";
    str << RowTupleStringer<RowTuple>::string(rows);
    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}

SQLLIB_NS_END

#endif
