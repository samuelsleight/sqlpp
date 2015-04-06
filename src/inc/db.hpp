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
#include <memory>

SQLLIB_NS

template<typename>
class Table;

class DB : public std::enable_shared_from_this<DB> {
public:
    using Ptr = std::shared_ptr<DB>;

    virtual ~DB();

#ifndef NO_SQLITE3
    static Ptr sqlite3(std::string filename);
#endif

    std::shared_ptr<Table<Nil>> addTable(std::string name);

    virtual void executeCreate(std::string sql) = 0;

private:
    template<typename>
    friend class Table;
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

    DB();

    static Ptr connect(std::string filename);

    void executeCreate(std::string sql);
};

#endif // NO_SQLITE3

SQLLIB_SQLITE3_NS_END

SQLLIB_NS_END

#endif // SQLLIB_DB_HPP
