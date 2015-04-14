#ifndef SQLLIB_CONNECTION_HPP
#define SQLLIB_CONNECTION_HPP

#include "defines.hpp"
#include "statement.hpp"

#include <memory>

#ifndef SQLLIB_NO_SQLITE3
#include <sqlite3.h>
#endif

SQLLIB_NS

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS
class SQLite3Connection;
SQLLIB_SQLITE3_NS_END
#endif

template<typename...>
class Create;

template<typename, typename>
class Insert;

template<typename, int...>
class SelectImpl;

class Connection {
public:
    virtual ~Connection();

#ifndef SQLLIB_NO_SQLITE3
    static std::shared_ptr<SQLLIB_SQLITE3_NS_(SQLite3Connection)> sqlite3(std::string dbfile);
#endif

private:
    template<typename...>
    friend class Create;

    template<typename, typename>
    friend class Insert;

    template<typename, int...>
    friend class SelectImpl;

    virtual void execute(std::string sql) = 0;
    virtual std::shared_ptr<Statement> prepareSQL(std::string sql) = 0;
};

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS

class SQLite3Connection : public SQLLIB_NS_(Connection), public std::enable_shared_from_this<SQLite3Connection> {
public:
    using Ptr = std::shared_ptr<SQLite3Connection>;

    ~SQLite3Connection();

private:
    friend class SQLLIB_NS_(Connection);

    ::sqlite3* db;

    SQLite3Connection(std::string dbfile);

    void execute(std::string sql) override;
    std::shared_ptr<SQLLIB_NS_(Statement)> prepareSQL(std::string sql);
};

SQLLIB_SQLITE3_NS_END
#endif

SQLLIB_NS_END

#endif
