#ifndef SQLLIB_INSERT_HPP
#define SQLLIB_INSERT_HPP

#include "defines.hpp"
#include "query.hpp"
#include "templates.hpp"
#include "field.hpp"

#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <memory>

SQLLIB_NS

template<typename... TableTypes>
class TableSetImpl;

template<typename... TableTypes>
class TableSet;

template<typename TableType, typename FieldTuple>
class Insert;

template<typename TableType, int...IDs, typename... Types>
class Insert<TableType, std::tuple<Field<IDs, Types>...>> : public Query, public std::enable_shared_from_this<Insert<TableType, std::tuple<Field<IDs, Types>...>>> {
public:
    using TypeTuple = std::tuple<Types...>;
    using FieldTuple = std::tuple<Field<IDs, Types>...>;
    using Ptr = std::shared_ptr<Insert<TableType, FieldTuple>>;

    auto values(typename ValueTypeOf<Types>::Type... vals);

    std::string sql();
    void execute(std::shared_ptr<Connection> connection);

private:
    template<typename... TableTypes>
    friend class TableSetImpl;

    template<typename... TableTypes>
    friend class TableSet;

    TableType table;
    FieldTuple fieldTuple;

    std::vector<TypeTuple> valueList;

    Insert(TableType table, FieldTuple fieldTuple);
};

SQLLIB_NS_END

#include "tpp/insert.tpp"

#endif
