#ifndef SQLLIB_TABLE_TPP
#define SQLLIB_TABLE_TPP

#include "../field.hpp"
#include "../templates.hpp"

SQLLIB_NS

template<int ID, typename... FieldTypes>
Table<ID, FieldTypes...>::Table(std::string name, FieldTuple fieldTuple)
    : name(name), fieldTuple(fieldTuple) {}

template<int ID, typename... FieldTypes>
constexpr int Table<ID, FieldTypes...>::getID() {
    return id;
}

template<int ID, typename... FieldTypes>
std::string Table<ID, FieldTypes...>::getName() {
    return name;
}

template<int ID, typename... FieldTypes>
std::string Table<ID, FieldTypes...>::getCreateSQL(bool ine) {
    std::ostringstream str;

    str << "CREATE TABLE " << (ine ? "IF NOT EXISTS " : " ");
    str << name << "(";
    str << tupleFold(fieldTuple, std::string(""), [](std::string acc, auto field) -> std::string {
        return field.getDefinition() + (acc == "" ? "" : ", ") + acc;
    });
    str << ")";

    return str.str();
}

template<int ID, typename... FieldTypes>
template<int... IDs>
auto Table<ID, FieldTypes...>::fields() {
    return TupleFromIDs<FieldTuple, List<IDs...>>::make(fieldTuple);
}

template<int ID, typename... FieldTypes>
template<int FID>
auto Table<ID, FieldTypes...>::field() {
    return std::get<IndexFromID<FieldTuple, FID>::value>(fieldTuple);
}

template<int ID, typename... FieldTypes>
template<int FieldID, typename FieldType>
auto Table<ID, FieldTypes...>::addField(std::string name) {
    auto field = Field<FieldID, FieldType>(name);
    auto tuple = std::tuple_cat(fieldTuple, std::make_tuple(field));

    using NewTable = Table<ID, FieldTypes..., decltype(field)>;

    return NewTable(this->name, tuple);
}

SQLLIB_NS_END

#endif
