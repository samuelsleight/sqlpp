#ifndef SQLLIB_TABLESET_TPP
#define SQLLIB_TABLESET_TPP

#include "../create.hpp"

SQLLIB_NS

template<typename... TableTypes>
TableSet<TableTypes...>::TableSet(typename TableSetImpl<TableTypes...>::TableTuple tableTuple)
    : TableSetImpl<TableTypes...>(tableTuple) {}

template<typename TableType>
TableSet<TableType>::TableSet(typename TableSetImpl<TableType>::TableTuple tableTuple)
    : TableSetImpl<TableType>(tableTuple) {}

template<typename... TableTypes>
TableSetImpl<TableTypes...>::TableSetImpl(TableTuple tableTuple)
    : tableTuple(tableTuple) {}

template<typename... TableTypes>
TableSetImpl<TableTypes...>::~TableSetImpl() {}

template<typename... TableTypes>
auto TableSetImpl<TableTypes...>::create(bool ine) {
    using NewCreate = Create<TableTypes...>;
    using NewPtr = typename NewCreate::Ptr;

    return NewPtr(new NewCreate(tableTuple, ine));
};

template<typename TableType>
auto TableSet<TableType>::insert() {

};

template<typename... TableTypes>
struct TableSetFromTuple<std::tuple<TableTypes...>> {
    using type = TableSet<TableTypes...>;
};

SQLLIB_NS_END

#endif
