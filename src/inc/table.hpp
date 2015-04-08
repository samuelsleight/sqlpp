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

template<int TID, typename FieldTuple, typename PKey, typename FKeys>
class Table : public std::enable_shared_from_this<Table<TID, FieldTuple, PKey, FKeys>> {
public:
    using Ptr = std::shared_ptr<Table<TID, FieldTuple, PKey, FKeys>>;

    template<typename T, int N>
    auto addField(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");
        static_assert(!FieldTupleIDExists<FieldTuple, N>::value, "row identifier must be unique");

        auto newTuple = std::tuple_cat(fieldTuple, std::make_tuple(Field<T, N, TID>(name, rowName)));

        using NewTuple = decltype(newTuple);
        using NewTable = Table<TID, NewTuple, PKey, FKeys>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, newTuple, foreignKeys));
    }

    template<int... IDs>
    auto setPrimaryKey() const {
        using Key = PrimaryKey<IDs...>;
        using NewTable = Table<TID, FieldTuple, Key, FKeys>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, fieldTuple, foreignKeys));
    }

    template<int... IDs, typename OtherFieldTuple>
    auto addForeignKey(OtherFieldTuple tuple) {
        auto keyFields = MakeFieldTuple<FieldTuple, IDs...>::make(fieldTuple);

        using KeyFields = decltype(keyFields);

        static_assert(FieldTypesMatch<KeyFields, OtherFieldTuple>::value, "foreign key type does not match referenced fields");

        auto key = std::make_tuple(keyFields, tuple);
        auto newFKeys = std::tuple_cat(foreignKeys, std::make_tuple(key));

        using NewFKeys = decltype(newFKeys);
        using NewTable = Table<TID, FieldTuple, PKey, NewFKeys>;
        using NewPtr = typename NewTable::Ptr;

        return NewPtr(new NewTable(db, name, fieldTuple, newFKeys));
    }

    const std::string getName() const {
        return name;
    }

    template<int... IDs>
    auto fields() {
        return MakeFieldTuple<FieldTuple, IDs...>::make(fieldTuple);
    }

    auto insert() {
        using InsertTypes = decltype(MakeTypeTuple<FieldTuple>::make(fieldTuple));
        using InsertType = typename InsertFromTuple<FieldTuple, InsertTypes>::type;

        return typename InsertType::Ptr(new InsertType(db, name, fieldTuple));
    }

    template<int... FieldIDs>
    auto insert() {
        auto insertFields = MakeFieldTuple<FieldTuple, FieldIDs...>::make(fieldTuple);
        using InsertTypes = decltype(MakeTypeTuple<decltype(insertFields)>::make(insertFields));
        using InsertType = typename InsertFromTuple<decltype(insertFields), InsertTypes>::type;

        return typename InsertType::Ptr(new InsertType(db, name, insertFields));
    }

    typename Select<FieldTuple>::Ptr select() {
        return typename Select<FieldTuple>::Ptr(new Select<FieldTuple>(db, name, fieldTuple));
    }

    Ptr create(bool ifNotExists = true);

private:
    friend class DB;

    template<int, typename, typename, typename>
    friend class Table;

    DB* db;
    std::string name;
    FieldTuple fieldTuple;

    FKeys foreignKeys;

    Table(DB* db, std::string name, FieldTuple fieldTuple = std::tuple<>(), FKeys foreignKeys = std::tuple<>())
        : db(db), name(name), fieldTuple(fieldTuple), foreignKeys(foreignKeys) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<int TID, typename FieldTuple, typename PKey, typename FKeys>
typename Table<TID, FieldTuple, PKey, FKeys>::Ptr Table<TID, FieldTuple, PKey, FKeys>::create(bool ifNotExists) {
    std::ostringstream str;
    str << "CREATE TABLE ";

    if(ifNotExists) {
        str << "IF NOT EXISTS ";
    }
   
    str << name << "(";
    str << FieldTupleStringer<FieldTuple>::string(fieldTuple);
    str << PrimaryKeyStringer<FieldTuple, PKey>::string(fieldTuple);
    str << ForeignKeyStringer<FKeys>::string(foreignKeys);
    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}

SQLLIB_NS_END

#endif
