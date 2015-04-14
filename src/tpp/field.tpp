#ifndef SQLLIB_FIELD_TPP
#define SQLLIB_FIELD_TPP

SQLLIB_NS

template<int ID, typename Type>
Field<ID, Type>::Field(std::string name)
    : name(name) {}

template<int ID, typename Type>
constexpr int Field<ID, Type>::getID() {
    return id;
}

template<int ID, typename Type>
std::string Field<ID, Type>::getName() {
    return name;
}

template<int ID, typename Type>
std::string Field<ID, Type>::getDefinition() {
    return name + " " + Type::getSQLType();
}

template<int ID, typename Type>
auto Field<ID, Type>::getValue(std::shared_ptr<Statement> conn, int index) {
    return ValueType::getValue(conn, index);
}

SQLLIB_NS_END

#endif
