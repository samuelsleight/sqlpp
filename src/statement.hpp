#ifndef SQLLIB_STATEMENT_HPP
#define SQLLIB_STATEMENT_HPP

#include "defines.hpp"

#include <memory>
#include <string>
#include <sstream>

#ifndef SQLLIB_NO_SQLITE3
#include <sqlite3.h>
#endif

SQLLIB_NS

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS
class SQLite3Statement;
class SQLite3Connection;
SQLLIB_SQLITE3_NS_END
#endif

class Connection;

class SQLLIB_API Statement {
public:
    virtual ~Statement();

    virtual void bindInt(int index, int value) = 0;
    virtual void bindString(int index, std::string value) = 0;

    virtual void execute() = 0;
    virtual bool select() = 0;

    virtual int intValue(int index) = 0;
    virtual std::string stringValue(int index) = 0;

private:
    friend class Connection;

#ifndef SQLLIB_NO_SQLITE3
    friend class SQLite3Connection;
#endif
};

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS

class SQLLIB_API SQLite3Statement : public SQLLIB_NS_(Statement), public std::enable_shared_from_this<SQLite3Statement> {
public:
    using Ptr = std::shared_ptr<SQLite3Statement>;

    ~SQLite3Statement();

private:
    friend class SQLite3Connection;

    ::sqlite3_stmt* stmt;

    SQLite3Statement(::sqlite3_stmt* stmt);

    void bindInt(int index, int value);
    void bindString(int index, std::string value);

    void execute();
    bool select();

    int intValue(int index);
    std::string stringValue(int index);
};

SQLLIB_SQLITE3_NS_END
#endif

SQLLIB_NS_END

#endif
