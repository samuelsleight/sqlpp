#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "type.hpp"
#include "field.hpp"
#include "insert.hpp"
#include "select.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <memory>
#include <type_traits>

SQLLIB_NS

class DB;

template<typename FieldTuple, typename... FieldTypes>
class Table : public std::enable_shared_from_this<Table<FieldTuple, FieldTypes...>> {
public:
    using Ptr = std::shared_ptr<Table<FieldTuple, FieldTypes...>>;

    template<typename T, int N>
    auto addField(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");
        static_assert(!FieldTupleIDExists<FieldTuple, N>::value, "row identifier must be unique");

        auto newTuple = std::tuple_cat(rows, std::make_tuple(Field<T, N>(rowName)));

        using NewTuple = decltype(newTuple);
        using NewTable = Table<NewTuple, FieldTypes..., T>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, newTuple));
    }

    const std::string getName() const {
        return name;
    }

    template<int N>
    auto getField() {
        static_assert(FieldTupleIDExists<FieldTuple, N>::value, "row identifier does not exist");
        return FieldTupleGet<FieldTuple, N>::get(rows);
    }

    typename Insert<FieldTuple, FieldTypes...>::Ptr insert() {
        return typename Insert<FieldTuple, FieldTypes...>::Ptr(new Insert<FieldTuple, FieldTypes...>(db, name, rows));
    }

    template<int... FieldIDs>
    auto insert() {
        auto insertFields = MakeFieldTuple<FieldTuple, FieldIDs...>::make(rows);
        using InsertTypes = decltype(MakeTypeTuple<decltype(insertFields)>::make(insertFields));
        using InsertType = typename InsertFromTuple<decltype(insertFields), InsertTypes>::type;

        return typename InsertType::Ptr(new InsertType(db, name, insertFields));
    }

    typename Select<FieldTuple, FieldTypes...>::Ptr select() {
        return typename Select<FieldTuple, FieldTypes...>::Ptr(new Select<FieldTuple, FieldTypes...>(db, name, rows));
    }

    Ptr create();

private:
    friend class DB;

    template<typename, typename...>
    friend class Table;

    DB* db;
    std::string name;
    FieldTuple rows;

    Table(DB* db, std::string name, FieldTuple rows = std::tuple<>())
        : db(db), name(name), rows(rows) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple, typename... FieldTypes>
typename Table<FieldTuple, FieldTypes...>::Ptr Table<FieldTuple, FieldTypes...>::create() {
    std::ostringstream str;
    str << "CREATE TABLE " << name << "(";
    str << FieldTupleStringer<FieldTuple>::string(rows);
    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}

SQLLIB_NS_END

#endif
