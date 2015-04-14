SQLLIB_NS

template<typename TableTuple, int... Ns>
SelectImpl<TableTuple, Ns...>::~SelectImpl() {}

template<typename TableTuple, int... Ns>
SelectImpl<TableTuple, Ns...>::SelectImpl(TableTuple tableTuple)
    : tableTuple(tableTuple), callbackFunc([](auto... ps){}) {}

template<typename TableTuple, int... Ns>
Select<TableTuple, Ns...>::Select(TableTuple tableTuple)
    : SelectImpl<TableTuple, Ns...>(tableTuple) {}

template<typename TableTuple, typename Where, int... Ns>
SelectWhere<TableTuple, Where, Ns...>::SelectWhere(TableTuple tableTuple)
    : SelectImpl<TableTuple, Ns...>(tableTuple) {}

template<typename TableTuple, int... Ns>
template<int Index, int FieldID>
auto Select<TableTuple, Ns...>::select() {
    using NewSelect = Select<TableTuple, Ns..., Index, FieldID>;
    using NewPtr = typename NewSelect::Ptr;

    return NewPtr(new NewSelect(this->tableTuple));
}

template<typename TableTuple, int... Ns>
template<typename Where>
auto Select<TableTuple, Ns...>::where() {
    using NewSelect = SelectWhere<TableTuple, Where, Ns...>;
    using NewPtr = typename NewSelect::Ptr;

    return NewPtr(new NewSelect(this->tableTuple));
}

template<typename TableTuple, int... Ns>
std::string Select<TableTuple, Ns...>::sql() {
    std::ostringstream str;

    str << "SELECT ";
    str << SelectFields<TableTuple, Ns...>::write(this->tableTuple);
    str << " FROM ";

    int i = std::tuple_size<TableTuple>::value - 1;
    str << tupleFold(this->tableTuple, std::string(""), [&i](std::string acc, auto table) -> std::string {
        return table.getName() + " t" + std::to_string(i--) + (acc == "" ? ";" : ", ") + acc; 
    });

    return str.str();
}

template<typename TableTuple, typename Where, int... Ns>
std::string SelectWhere<TableTuple, Where, Ns...>::sql() {
    std::ostringstream str;

    str << "SELECT ";
    str << SelectFields<TableTuple, Ns...>::write(this->tableTuple);
    str << " FROM ";

    int i = std::tuple_size<TableTuple>::value - 1;
    str << tupleFold(this->tableTuple, std::string(""), [&i](std::string acc, auto table) -> std::string {
        return table.getName() + " t" + std::to_string(i--) + (acc == "" ? "" : ", ") + acc; 
    });

    str << " WHERE ";
    str << WriteWhere<TableTuple, Where>::write(this->tableTuple);
    str << ";";

    return str.str();
}

template<typename TableTuple, int... Ns>
auto SelectImpl<TableTuple, Ns...>::callback(CallbackType func) {
    this->callbackFunc = func;

    return this->shared_from_this();
}

template<typename TableTuple, int... Ns>
void SelectImpl<TableTuple, Ns...>::execute(std::shared_ptr<Connection> connection) {
    auto stmt = connection->prepareSQL(sql());

    auto tuple = SelectFieldTuple<TableTuple, Ns...>::make(tableTuple);

    while(stmt->select()) {
        int i = std::tuple_size<decltype(tuple)>::value - 1;
        auto values = tupleFold(tuple, std::make_tuple(), [&i, &stmt](auto acc, auto field) {
            return std::tuple_cat(std::make_tuple(field.getValue(stmt, i--)), acc);
        });

        callWithTuple(callbackFunc, values);
    }
}

SQLLIB_NS_END
