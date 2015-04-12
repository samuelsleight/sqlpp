#include "connection.hpp"
#include "exception.hpp"

#include <iostream>

SQLLIB_NS

Connection::~Connection() {}

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS_(SQLite3Connection)::Ptr Connection::sqlite3(std::string dbfile) {
    return SQLLIB_SQLITE3_NS_(SQLite3Connection)::Ptr(new SQLLIB_SQLITE3_NS_(SQLite3Connection)(dbfile));
}

SQLLIB_SQLITE3_NS
SQLite3Connection::SQLite3Connection(std::string dbfile) {
    int err = ::sqlite3_open(dbfile.c_str(), &db);
    ::sqlite3_extended_result_codes(db, true);

    if(err != SQLITE_OK) {
        throw Exception(::sqlite3_errstr(err));
    }
}

SQLite3Connection::~SQLite3Connection() {
    ::sqlite3_close(db);
}

void SQLite3Connection::execute(std::string sql) {
    char* err;
    ::sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);

    if(err != nullptr) {
        std::string msg(err);
        ::sqlite3_free(err);
        throw Exception(msg);
    }
}

SQLLIB_SQLITE3_NS_END
#endif

SQLLIB_NS_END
