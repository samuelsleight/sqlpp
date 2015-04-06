#ifndef SQLLIB_TABLE_HPP
#define SQLLIB_TABLE_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "type.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <type_traits>

SQLLIB_NS

class DB;

template<typename RowTypeList>
class Table : public std::enable_shared_from_this<Table<RowTypeList>> {
public:
    using Ptr = std::shared_ptr<Table<RowTypeList>>;

    template<typename T>
    typename Table<Cons<T, RowTypeList>>::Ptr addRow(std::string rowName) const {
        static_assert(std::is_base_of<Type, T>::value, "table row type must extend Type");

        using NewList = Cons<T, RowTypeList>;
        using NewTable = Table<NewList>;
        using NewPtr = typename NewTable::Ptr;

        std::vector<std::string> newRows = rows;
        newRows.push_back(rowName);

        return NewPtr(new NewTable(db, name, newRows));
    }

    const std::vector<std::string>& getRowNames() const {
        return rows;
    }

    const std::string getName() const {
        return name;
    }

    Table<RowTypeList>::Ptr create();

private:
    friend class DB;

    template<typename>
    friend class Table;

    DB* db;
    std::string name;
    std::vector<std::string> rows;

    Table(DB* db, std::string name, std::vector<std::string> rows = std::vector<std::string>())
        : db(db), name(name), rows(rows) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename RowTypeList>
typename Table<RowTypeList>::Ptr Table<RowTypeList>::create() {
    std::ostringstream str;
    str << "CREATE TABLE " << name << "(";

    std::vector<std::string> rowTypes = ParseTypeList<RowTypeList>::parse();
    for(unsigned int i = 0; i < rows.size(); ++i) {
        if(i != 0) {
            str << ", ";
        }

        str << rows[i] << " " << rowTypes[i];
    }

    str << ")";

    db->executeCreate(str.str());

    return this->shared_from_this();
}


SQLLIB_NS_END

#endif
