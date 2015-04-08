#ifndef SQLLIB_SELECT_HPP
#define SQLLIB_SELECT_HPP

#include "defines.hpp"
#include "results.hpp"

#include <tuple>
#include <memory>

SQLLIB_NS

class DB;

template<typename, typename, typename...>
class Table;

template<typename FieldTuple, typename... FieldTypes>
class Select : public std::enable_shared_from_this<Select<FieldTuple, FieldTypes...>> {
public:
    using Ptr = std::shared_ptr<Select<FieldTuple, FieldTypes...>>;

    SelectResult<FieldTuple, FieldTypes...> execute();

private:
    template<typename, typename, typename...>
    friend class Table;

    DB* db;
    std::string table;
    FieldTuple fields;

    Select(DB* db, std::string table, FieldTuple fields)
        : db(db), table(table), fields(fields) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple, typename... FieldTypes>
SelectResult<FieldTuple, FieldTypes...> Select<FieldTuple, FieldTypes...>::execute() {
    std::ostringstream str;
    str << "SELECT ";
    str << FieldTupleNames<FieldTuple>::string(fields);
    str << " FROM " << table << ";";

    db->prepareQuery(str.str());
    db->executePreparedSelect();
    return SelectResult<FieldTuple, FieldTypes...>(db, fields);
}

SQLLIB_NS_END

#endif
