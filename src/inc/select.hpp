#ifndef SQLLIB_SELECT_HPP
#define SQLLIB_SELECT_HPP

#include "defines.hpp"
#include "results.hpp"

#include <tuple>
#include <memory>

SQLLIB_NS

class DB;

template<int, typename, typename, typename>
class Table;

template<typename FieldTuple>
class Select : public std::enable_shared_from_this<Select<FieldTuple>> {
public:
    using Ptr = std::shared_ptr<Select<FieldTuple>>;

    SelectResult<FieldTuple> execute();

private:
    template<int, typename, typename, typename>
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

template<typename FieldTuple>
SelectResult<FieldTuple> Select<FieldTuple>::execute() {
    std::ostringstream str;
    str << "SELECT ";
    str << FieldTupleNames<FieldTuple>::string(fields);
    str << " FROM " << table << ";";

    db->prepareQuery(str.str());
    db->executePreparedSelect();
    return SelectResult<FieldTuple>(db, fields);
}

SQLLIB_NS_END

#endif
