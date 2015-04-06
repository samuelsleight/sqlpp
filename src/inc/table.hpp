#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "type.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <memory>
#include <type_traits>

SQLLIB_NS

class DB;

template<typename T>
class Row {
private:
    std::string name;

public:
    using Type = T;

    Row(std::string name)
        : name(name) {}

    std::string getName() const {
        return name;
    }
};

template<typename RowTypeList, typename RowTuple>
class Table : public std::enable_shared_from_this<Table<RowTypeList, RowTuple>> {
public:
    using Ptr = std::shared_ptr<Table<RowTypeList, RowTuple>>;

    template<typename T>
    auto addRow(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");

        auto newTuple = std::tuple_cat(rows, std::make_tuple(Row<T>(rowName)));

        using NewList = typename ListAdd<RowTypeList, T>::list;
        using NewTable = Table<NewList, decltype(newTuple)>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, newTuple));
    }

    const std::string getName() const {
        return name;
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

template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct RowTupleStringer {
    static std::string string(Tuple tuple) {
        std::ostringstream str;
        str << RowTupleStringer<Tuple, N - 1>::string(tuple);
        str << ", " << std::get<N>(tuple).getName() << " " << std::remove_reference<decltype(std::get<N>(tuple))>::type::Type::sqlType();
    
        return str.str();
    }
};

template<typename Tuple>
struct RowTupleStringer<Tuple, 0> {
    static std::string string(Tuple tuple) {
        std::ostringstream str;
        str << std::get<0>(tuple).getName() << " " << std::remove_reference<decltype(std::get<0>(tuple))>::type::Type::sqlType();

        return str.str();
    }
};

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
