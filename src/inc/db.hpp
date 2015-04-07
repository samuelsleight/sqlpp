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
#include <tuple>
#include <memory>

SQLLIB_NS

template<typename, typename...>
class Table;

template<typename, typename...>
class Insert;

class DB : public std::enable_shared_from_this<DB> {
public:
    using Ptr = std::shared_ptr<DB>;

    virtual ~DB();

#ifndef NO_SQLITE3
    static Ptr sqlite3(std::string filename);
#endif

    std::shared_ptr<Table<std::tuple<>>> addTable(std::string name);

    virtual void bindString(int index, std::string value) = 0;
    virtual void bindInteger(int index, int value) = 0;

protected:
    template<typename, typename...>
    friend class Table;

    template<typename, typename...>
    friend class Insert;

    virtual void executeCreate(std::string sql) = 0;

    virtual void beginTransaction() = 0;
    virtual void prepareQuery(std::string sql) = 0;

    virtual void executePreparedInsert() = 0;

    virtual void cleanPreparedQuery() = 0;
    virtual void commitTransaction() = 0;
};

#ifndef NO_SQLITE3

SQLLIB_SQLITE3_NS

class DB : public SQLLIB_NS_(DB) {
public:
    using Ptr = std::shared_ptr<DB>;

    ~DB();

private:
    friend class SQLLIB_NS_(DB);

    ::sqlite3* db;
    ::sqlite3_stmt* stmt;

    DB();

    static Ptr connect(std::string filename);

    void executeCreate(std::string sql);

    void beginTransaction();
    void prepareQuery(std::string sql);

    void bindString(int index, std::string value);
    void bindInteger(int index, int value);

    void executePreparedInsert();

    void cleanPreparedQuery();
    void commitTransaction();
};

#endif // NO_SQLITE3

SQLLIB_SQLITE3_NS_END

SQLLIB_NS_END

#endif // SQLLIB_DB_HPP
