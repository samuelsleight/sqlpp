#ifndef SQLLIB_TABLESET_HPP
#define SQLLIB_TABLESET_HPP

#include "defines.hpp"

#include <tuple>
#include <memory>

SQLLIB_NS

template<int, typename...>
class DB;

template<typename... TableTypes>
class TableSetImpl {
public:
    using TableTuple = std::tuple<TableTypes...>;

    virtual ~TableSetImpl();

    auto create(bool ine = true);

protected:
    template<int, typename...>
    friend class DB;

    TableTuple tableTuple;

    TableSetImpl(TableTuple tableTuple);
};

template<typename... TableTypes>
class TableSet : public TableSetImpl<TableTypes...> {
public:
    using Ptr = std::shared_ptr<TableSet<TableTypes...>>;

private:
    template<int, typename...>
    friend class DB;

    TableSet(typename TableSetImpl<TableTypes...>::TableTuple tableTuple);
};

template<typename TableType>
class TableSet<TableType> : public TableSetImpl<TableType> {
public:
    using Ptr = std::shared_ptr<TableSet<TableType>>;

    template<int... FieldIDs>
    auto insert();

private:
    template<int, typename...>
    friend class DB;

    TableSet(typename TableSetImpl<TableType>::TableTuple tableTuple);
};

template<typename>
struct TableSetFromTuple;

SQLLIB_NS_END

#include "tpp/tableset.tpp"

#endif
