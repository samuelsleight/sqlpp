#ifndef SQLLIB_CREATE_TPP
#define SQLLIB_CREATE_TPP

#include "../templates.hpp"

#include <iostream>

SQLLIB_NS

template<typename... TableTypes>
Create<TableTypes...>::Create(TableTuple tableTuple, bool ine)
    : tableTuple(tableTuple), ine(ine) {}

template<typename... TableTypes>
std::string Create<TableTypes...>::sql() {
    return tupleFold(tableTuple, std::string(""), [ine = this->ine](std::string acc, auto table) -> std::string {
        return table.getCreateSQL(ine) + (acc == "" ? ";" : "; ") + acc;
    });
}

template<typename... TableTypes>
void Create<TableTypes...>::execute(std::shared_ptr<Connection> connection) {
    connection->execute(sql());
}

SQLLIB_NS_END

#endif
