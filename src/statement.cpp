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
    int err = ::sqlite3_bind_text(stmt, index, value.c_str(), value.size(), SQLITE_TRANSIENT);

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

bool SQLite3Statement::select() {
    int err = ::sqlite3_step(stmt);

    if(err == SQLITE_ROW) {
        return true;
    }

    if(err == SQLITE_DONE) {
        return false;
    }

    throw Exception(::sqlite3_errstr(err));
}

int SQLite3Statement::intValue(int index) {
    return ::sqlite3_column_int(stmt, index);
}

std::string SQLite3Statement::stringValue(int index) {
//    return std::string(reinterpret_cast<const char*>(::sqlite3_column_text(stmt, index)));
	std::ostringstream str;

	str << ::sqlite3_column_text(stmt, index);
	return str.str();
}

SQLLIB_SQLITE3_NS_END
#endif

SQLLIB_NS_END
