#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"

#include <string>
#include <sstream>
#include <memory>

SQLLIB_NS

class EmptyDB;

template<int, typename...>
class DB;

template<int ID, typename... FieldTypes>
class Table {
public:
    using Ptr = std::shared_ptr<Table<ID, FieldTypes...>>;
    using FieldTuple = std::tuple<FieldTypes...>;

    static constexpr int getID();
    std::string getName();

    std::string getCreateSQL(bool ine = true);

private:
    friend class EmptyDB;

    template<int, typename...>
    friend class DB;

    template<int, typename...>
    friend class Table;

    std::string name;
    static constexpr int id = ID;

    FieldTuple fieldTuple;

    Table(std::string name, FieldTuple fieldTuple);

    template<int FieldID, typename FieldType>
    auto addField(std::string name);
};

SQLLIB_NS_END

#include "tpp/table.tpp"

#endif
