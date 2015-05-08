#ifndef SQLLIB_DB_HPP
#define SQLLIB_DB_HPP

#include "defines.hpp"

#include <string>
#include <tuple>
#include <memory>

SQLLIB_NS

class SQLLIB_API EmptyDB : public std::enable_shared_from_this<EmptyDB> {
public:
    using Ptr = std::shared_ptr<EmptyDB>;

    template<int ID>
    auto addTable(std::string name);

    friend SQLLIB_API Ptr database();

private:
    EmptyDB();
};

template<int ActiveTableID, typename... Tables>
class DB : public std::enable_shared_from_this<DB<ActiveTableID, Tables...>> {
public:
    using Ptr = std::shared_ptr<DB<ActiveTableID, Tables...>>;
    using TableTuple = std::tuple<Tables...>;

    template<int ID>
    auto addTable(std::string name);

    template<int FieldID, typename FieldType>
    auto addField(std::string name);

    template<int... IDs>
    auto tables();

private:
    friend class EmptyDB;

    template<int, typename...>
    friend class DB;

    TableTuple tableTuple;

    DB(TableTuple tableTuple);
};

template<int, typename>
struct DBFromTuple;

SQLLIB_NS_END

#include "tpp/db.tpp"

SQLLIB_NS

SQLLIB_API EmptyDB::Ptr database();

SQLLIB_NS_END

#endif
