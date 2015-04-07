#ifndef SQLLIB_INSERT_HPP
#define SQLLIB_INSERT_HPP

#include "defines.hpp"
#include "row.hpp"

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

template<typename RowTuple, typename... RowTypes>
class Insert : public std::enable_shared_from_this<Insert<RowTuple, RowTypes...>> {
public:
    using Ptr = std::shared_ptr<Insert<RowTuple, RowTypes...>>;
    using ValueTuple = std::tuple<RowTypes...>;

    Ptr values(RowTypes... rowValues) {
        valueList.push_back(std::make_tuple(rowValues...));
        return this->shared_from_this();
    }

    void execute();

private:
    template<typename, typename...>
    friend class Table;

    DB* db;
    std::string table;
    RowTuple rows;

    std::vector<ValueTuple> valueList;

    Insert(DB* db, std::string table, RowTuple rows)
        : db(db), table(table), rows(rows) {}
};

template<typename RowTuple, typename TypeTuple>
class InsertFromTuple;

template<typename RowTuple, typename... RowTypes>
class InsertFromTuple<RowTuple, std::tuple<RowTypes...>> {
public:
    using type = Insert<RowTuple, RowTypes...>;
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename RowTuple, typename... RowTypes>
void Insert<RowTuple, RowTypes...>::execute() {
    std::ostringstream str;
    str << "INSERT INTO " << table << "(";
    str << RowTupleNames<RowTuple>::string(rows);
    str << ") VALUES(";

    for(int i = 0; i < std::tuple_size<RowTuple>::value; ++i) {
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
