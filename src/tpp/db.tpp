#ifndef SQLLIB_DB_TPP
#define SQLLIB_DB_TPP

#include "../table.hpp"
#include "../tableset.hpp"
#include "../templates.hpp"

SQLLIB_NS

template<int ID>
auto EmptyDB::addTable(std::string name) {
    using NewDB = DB<ID, Table<ID>>;
    using NewPtr = typename NewDB::Ptr;

    auto table = Table<ID>(name, std::tuple<>());
    auto tuple = std::make_tuple(table);

    return NewPtr(new NewDB(tuple));
}

template<int ActiveTableID, typename... Tables>
DB<ActiveTableID, Tables...>::DB(TableTuple tableTuple)
    : tableTuple(tableTuple) {}

template<int ActiveTableID, typename... Tables> 
template<int ID>
auto DB<ActiveTableID, Tables...>::addTable(std::string name) {
    using NewDB = DB<ID, Tables..., Table<ID>>;
    using NewPtr = typename NewDB::Ptr;

    auto table = Table<ID>(name, std::tuple<>());
    auto tuple = std::tuple_cat(tableTuple, std::make_tuple(table));

    return NewPtr(new NewDB(tuple));
}

template<int ActiveTableID, typename... Tables> 
template<int FieldID, typename FieldType>
auto DB<ActiveTableID, Tables...>::addField(std::string name) {
    constexpr int index = ::IndexFromID<TableTuple, ActiveTableID>::value;

    auto table = std::get<index>(tableTuple).template addField<FieldID, FieldType>(name);
    auto tuple = ::ReplaceIndex<TableTuple, decltype(table), index>::replace(tableTuple, table);

    using NewDB = typename DBFromTuple<ActiveTableID, decltype(tuple)>::type;
    using NewPtr = typename NewDB::Ptr;

    return NewPtr(new NewDB(tuple));
}

template<int ActiveTableID, typename... Tables> 
template<int... IDs>
auto DB<ActiveTableID, Tables...>::tables() {
    auto tuple = TupleFromIDs<TableTuple, List<IDs...>>::make(tableTuple);

    using NewSet = typename TableSetFromTuple<decltype(tuple)>::type;
    using NewPtr = typename NewSet::Ptr;

    return NewPtr(new NewSet(tuple));
}

template<int ActiveTableID, typename... TableTypes>
struct DBFromTuple<ActiveTableID, std::tuple<TableTypes...>> {
    using type = DB<ActiveTableID, TableTypes...>;
};

SQLLIB_NS_END

#endif
