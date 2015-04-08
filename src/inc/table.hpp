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

template<typename FieldTuple, typename PrimKey, typename... FieldTypes>
class Table : public std::enable_shared_from_this<Table<FieldTuple, PrimKey, FieldTypes...>> {
public:
    using Ptr = std::shared_ptr<Table<FieldTuple, PrimKey, FieldTypes...>>;

    template<typename T, int N>
    auto addField(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");
        static_assert(!FieldTupleIDExists<FieldTuple, N>::value, "row identifier must be unique");

        auto newTuple = std::tuple_cat(fields, std::make_tuple(Field<T, N>(rowName)));

        using NewTuple = decltype(newTuple);
        using NewTable = Table<NewTuple, PrimKey, FieldTypes..., T>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, newTuple));
    }

    template<int... IDs>
    auto setPrimaryKey() const {
        using Key = PrimaryKey<IDs...>;
        using NewTable = Table<FieldTuple, Key, FieldTypes...>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, fields));
    }

    const std::string getName() const {
        return name;
    }

    template<int ID>
    auto getField() {
        static_assert(FieldTupleIDExists<FieldTuple, ID>::value, "row identifier does not exist");
        return FieldTupleGet<FieldTuple, ID>::get(fields);
    }

    typename Insert<FieldTuple, FieldTypes...>::Ptr insert() {
        return typename Insert<FieldTuple, FieldTypes...>::Ptr(new Insert<FieldTuple, FieldTypes...>(db, name, fields));
    }

    template<int... FieldIDs>
    auto insert() {
        auto insertFields = MakeFieldTuple<FieldTuple, FieldIDs...>::make(fields);
        using InsertTypes = decltype(MakeTypeTuple<decltype(insertFields)>::make(insertFields));
        using InsertType = typename InsertFromTuple<decltype(insertFields), InsertTypes>::type;

        return typename InsertType::Ptr(new InsertType(db, name, insertFields));
    }

    typename Select<FieldTuple, FieldTypes...>::Ptr select() {
        return typename Select<FieldTuple, FieldTypes...>::Ptr(new Select<FieldTuple, FieldTypes...>(db, name, fields));
    }

    Ptr create();

private:
    friend class DB;

    template<typename, typename, typename...>
    friend class Table;

    DB* db;
    std::string name;
    FieldTuple fields;

    Table(DB* db, std::string name, FieldTuple fields = std::tuple<>())
        : db(db), name(name), fields(fields) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple, typename PrimKey, typename... FieldTypes>
typename Table<FieldTuple, PrimKey, FieldTypes...>::Ptr Table<FieldTuple, PrimKey, FieldTypes...>::create() {
    std::ostringstream str;
    str << "CREATE TABLE " << name << "(";
    str << FieldTupleStringer<FieldTuple>::string(fields);

    str << PrimaryKeyStringer<FieldTuple, PrimKey>::string(fields);

    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}

SQLLIB_NS_END

#endif
