#ifndef SQLLIB_INSERT_HPP
#define SQLLIB_INSERT_HPP

#include "defines.hpp"
#include "field.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <memory>

SQLLIB_NS

class DB;

template<typename, typename...>
class Table;

template<typename FieldTuple, typename... FieldTypes>
class Insert : public std::enable_shared_from_this<Insert<FieldTuple, FieldTypes...>> {
public:
    using Ptr = std::shared_ptr<Insert<FieldTuple, FieldTypes...>>;
    using ValueTuple = std::tuple<FieldTypes...>;

    Ptr values(FieldTypes... rowValues) {
        valueList.push_back(std::make_tuple(rowValues...));
        return this->shared_from_this();
    }

    void execute();

private:
    template<typename, typename...>
    friend class Table;

    DB* db;
    std::string table;
    FieldTuple rows;

    std::vector<ValueTuple> valueList;

    Insert(DB* db, std::string table, FieldTuple rows)
        : db(db), table(table), rows(rows) {}
};

template<typename FieldTuple, typename TypeTuple>
class InsertFromTuple;

template<typename FieldTuple, typename... FieldTypes>
class InsertFromTuple<FieldTuple, std::tuple<FieldTypes...>> {
public:
    using type = Insert<FieldTuple, FieldTypes...>;
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple, typename... FieldTypes>
void Insert<FieldTuple, FieldTypes...>::execute() {
    std::ostringstream str;
    str << "INSERT INTO " << table << "(";
    str << FieldTupleNames<FieldTuple>::string(rows);
    str << ") VALUES(";

    for(int i = 0; i < std::tuple_size<FieldTuple>::value; ++i) {
        if(i != 0) {
            str << ", ";
        }

        str << "?";
    }

    str << ");";

    db->beginTransaction();
    db->prepareQuery(str.str());

    for(const ValueTuple& valueTuple : valueList) {
        ValueTupleBind<ValueTuple>::bind(db, valueTuple);
        db->executePreparedInsert();
    }

    db->cleanPreparedQuery();
    db->commitTransaction();
}

SQLLIB_NS_END

#endif
