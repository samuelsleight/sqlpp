#ifndef SQLLIB_CREATE_HPP
#define SQLLIB_CREATE_HPP

#include "defines.hpp"
#include "query.hpp"

#include <tuple>
#include <memory>

SQLLIB_NS

template<typename... TableTypes>
class TableSetImpl;

template<typename... TableTypes>
class TableSet;

template<typename... TableTypes>
class Create : public Query,  public std::enable_shared_from_this<Create<TableTypes...>> {
public:
    using Ptr = std::shared_ptr<Create<TableTypes...>>;
    using TableTuple = std::tuple<TableTypes...>;

    std::string sql() override;
    void execute(std::shared_ptr<Connection> connection) override;

private:
    friend class TableSetImpl<TableTypes...>;
    friend class TableSet<TableTypes...>;

    TableTuple tableTuple;

    bool ine;

    Create(TableTuple tableTuple, bool ine);
};

SQLLIB_NS_END

#include "tpp/create.tpp"

#endif
