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
    return name;
}

SQLLIB_NS_END

#endif
