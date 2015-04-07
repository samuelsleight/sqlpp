#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "type.hpp"
#include "row.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <memory>
#include <type_traits>

SQLLIB_NS

class DB;

template<typename RowTypeList, typename RowTuple>
class Table : public std::enable_shared_from_this<Table<RowTypeList, RowTuple>> {
public:
    using Ptr = std::shared_ptr<Table<RowTypeList, RowTuple>>;

    template<typename T, int N>
    auto addRow(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");
        static_assert(!RowTupleIDExists<RowTuple, N>::value, "row identifier must be unique");

        auto newTuple = std::tuple_cat(rows, std::make_tuple(Row<T, N>(rowName)));

        using NewList = typename ListAdd<RowTypeList, T>::list;
        using NewTable = Table<NewList, decltype(newTuple)>;
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

    Table<RowTypeList, RowTuple>::Ptr create();

private:
    friend class DB;

    template<typename, typename>
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

template<typename RowTypeList, typename RowTuple>
typename Table<RowTypeList, RowTuple>::Ptr Table<RowTypeList, RowTuple>::create() {
    std::ostringstream str;
    str << "CREATE TABLE " << name << "(";
    str << RowTupleStringer<RowTuple>::string(rows);
    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}


SQLLIB_NS_END

#endif
