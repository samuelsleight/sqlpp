#include "statement.hpp"
#include "exception.hpp"

SQLLIB_NS

Statement::~Statement() {}

#ifndef SQLLIB_NO_SQLITE3
SQLLIB_SQLITE3_NS

SQLite3Statement::SQLite3Statement(::sqlite3_stmt* stmt)
    : stmt(stmt) {}

SQLite3Statement::~SQLite3Statement() {
    ::sqlite3_finalize(stmt);
}

void SQLite3Statement::bindInt(int index, int value) {
    int err = ::sqlite3_bind_int(stmt, index, value);

    if(err != SQLITE_OK) {
        throw Exception(::sqlite3_errstr(err));
    }
}

void SQLite3Statement::bindString(int index, std::string value) {
    int err = ::sqlite3_bind_text(stmt, index, value.c_str(), value.size(), SQLITE_STATIC);

    if(err != SQLITE_OK) {
        throw Exception(::sqlite3_errstr(err));
    }
}

void SQLite3Statement::execute() {
    int err = ::sqlite3_step(stmt);

    if(err != SQLITE_DONE) {
        throw Exception(::sqlite3_errstr(err));
    }

    ::sqlite3_reset(stmt);
}

SQLLIB_SQLITE3_NS_END
#endif

SQLLIB_NS_END
