#ifndef SQLLIB_DB_HPP
#define SQLLIB_DB_HPP

#include "defines.hpp"
#include "type.hpp"
#include "templates.hpp"

#ifndef NO_SQLITE3
    #include <sqlite3.h>
#endif

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <memory>

SQLLIB_NS

template<int, typename, typename, typename>
class Table;

template<typename, typename...>
class Insert;

template<typename, typename...>
class Select;

template<typename, typename...>
class SelectResult;

template<int ID>
using TableT = Table<ID, std::tuple<>, NoKey, std::tuple<>>;

class DB : public std::enable_shared_from_this<DB> {
public:
    using Ptr = std::shared_ptr<DB>;

    virtual ~DB();

#ifndef NO_SQLITE3
    static Ptr sqlite3(std::string filename);
#endif

    template<int ID>
    std::shared_ptr<TableT<ID>> addTable(std::string name);

    virtual void bindString(int index, std::string value) = 0;
    virtual void bindInteger(int index, int value) = 0;

    virtual std::string selectStringValue(int index) = 0;
    virtual int selectIntegerValue(int index) = 0;

    std::string getTableName(int id);

protected:
    template<int, typename, typename, typename>
    friend class Table;

    template<typename, typename...>
    friend class Insert;

    template<typename, typename...>
    friend class Select;

    template<typename, typename...>
    friend class SelectResult;

    std::map<int, std::string> tableNames;

    virtual void executeCreate(std::string sql) = 0;

    virtual void beginTransaction() = 0;
    virtual void prepareQuery(std::string sql) = 0;

    virtual void executePreparedInsert() = 0;
    virtual void executePreparedSelect() = 0;

    virtual bool selectNextRow() = 0;

    virtual void cleanPreparedQuery() = 0;
    virtual void commitTransaction() = 0;
};

SQLLIB_NS_END

#include "table.hpp"

SQLLIB_NS

template<int ID>
std::shared_ptr<TableT<ID>> DB::addTable(std::string name) {
    tableNames[ID] = name;

    return typename TableT<ID>::Ptr(new TableT<ID>(this, name));
}


#ifndef NO_SQLITE3

SQLLIB_SQLITE3_NS

class DB : public SQLLIB_NS_(DB) {
public:
    using Ptr = std::shared_ptr<DB>;

    ~DB();

private:
    friend class SQLLIB_NS_(DB);

    bool midSelect;

    ::sqlite3* db;
    ::sqlite3_stmt* stmt;

    DB();

    static Ptr connect(std::string filename);

    void executeCreate(std::string sql);

    void beginTransaction();
    void prepareQuery(std::string sql);

    void bindString(int index, std::string value);
    void bindInteger(int index, int value);

    std::string selectStringValue(int index);
    int selectIntegerValue(int index);

    void executePreparedInsert();
    void executePreparedSelect();

    bool selectNextRow();

    void cleanPreparedQuery();
    void commitTransaction();
};

#endif // NO_SQLITE3

SQLLIB_SQLITE3_NS_END

SQLLIB_NS_END

#endif // SQLLIB_DB_HPP
