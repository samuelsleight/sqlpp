#ifndef SQLLIB_INSERT_TPP
#define SQLLIB_INSERT_TPP

SQLLIB_NS

template<typename TableType, int... IDs, typename... Types>
Insert<TableType, std::tuple<Field<IDs, Types>...>>::Insert(TableType table, FieldTuple fieldTuple)
    : table(table), fieldTuple(fieldTuple) {}

template<typename TableType, int... IDs, typename... Types>
auto Insert<TableType, std::tuple<Field<IDs, Types>...>>::values(typename ValueTypeOf<Types>::Type... vals) {
    valueList.push_back(std::make_tuple(Types(vals)...));
    return this->shared_from_this();
}

template<typename TableType, int... IDs, typename... Types>
std::string Insert<TableType, std::tuple<Field<IDs, Types>...>>::sql() {
    std::ostringstream str;

    str << "INSERT INTO " << table.getName() << " VALUES(";
    for(int i = 0; i < sizeof...(Types); ++i) {
        if(i != 0) {
            str << ", ";
        }

        str << "?";
    }
    str << ");";

    return str.str();
}

template<typename TableType, int... IDs, typename... Types>
void Insert<TableType, std::tuple<Field<IDs, Types>...>>::execute(std::shared_ptr<Connection> connection) {
    auto stmt = connection->prepareSQL(sql());

    for(std::tuple<Types...> tuple : valueList) {
        int i = sizeof...(Types);
        tupleForEach(tuple, [&i, &stmt](auto value) {
            value.bind(i, stmt);
            i--;
        });
        stmt->execute();
    }
}

SQLLIB_NS_END

#endif
