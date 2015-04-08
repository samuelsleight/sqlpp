#include "inc/db.hpp"
#include "inc/table.hpp"

SQLLIB_NS

DB::~DB() {}

#ifndef NO_SQLITE3

DB::Ptr DB::sqlite3(std::string filename) {
    return SQLLIB_SQLITE3_NS_(DB)::connect(filename);
}

#endif

std::string DB::getTableName(int id) {
    return tableNames[id];
}

#ifndef NO_SQLITE3

SQLLIB_SQLITE3_NS

DB::DB() 
    : midSelect(false) {}

DB::~DB() {
    ::sqlite3_close(db);
}

DB::Ptr DB::connect(std::string filename) {
    DB* db = new DB();

    int err = ::sqlite3_open(filename.c_str(), &(db->db));

    if(err == SQLITE_OK) {
        return Ptr(db);
    } else {
        delete db;
        return nullptr;
    }
}

void DB::executeCreate(std::string sql) {
    if(!midSelect) {
        int err = ::sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

        if(err != SQLITE_OK) {
            throw err;
        }
    }
}

void DB::beginTransaction() {
    if(!midSelect) {
        int err = ::sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);

        if(err != SQLITE_OK) {
            throw err;
        }
    }
}

void DB::prepareQuery(std::string sql) {
    if(!midSelect) {
        int err = ::sqlite3_prepare(db, sql.c_str(), -1, &stmt, nullptr);

        if(err != SQLITE_OK) {
            throw err;
        }
    }
}

void DB::bindString(int index, std::string value) {
    int err = ::sqlite3_bind_text(stmt, index, value.c_str(), value.size(), SQLITE_STATIC);

    if(err != SQLITE_OK) {
        throw err;
    }
}
    
void DB::bindInteger(int index, int value) {
    int err = ::sqlite3_bind_int(stmt, index, value);

    if(err != SQLITE_OK) {
        throw err;
    }
}

void DB::executePreparedInsert() {
    if(!midSelect) {
        int err = ::sqlite3_step(stmt);
    
        if(err != SQLITE_DONE) {
            throw err;
        }

        ::sqlite3_reset(stmt);
    }
}

void DB::executePreparedSelect() {
    midSelect = true;
}

bool DB::selectNextRow() {
    if(midSelect) {
        int err = ::sqlite3_step(stmt);

        if(err == SQLITE_ROW) {
            return true;
        }

        if(err == SQLITE_DONE) {
            return false;
        }

        throw err;
    } else {
        return false;
    }
}

std::string DB::selectStringValue(int index) {
    return std::string(reinterpret_cast<const char*>(::sqlite3_column_text(stmt, index)));
}

int DB::selectIntegerValue(int index) {
    return ::sqlite3_column_int(stmt, index);
}

void DB::cleanPreparedQuery() {
    ::sqlite3_finalize(stmt);

    midSelect = false;
}

void DB::commitTransaction() {
    int err = ::sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);

    if(err != SQLITE_OK) {
        throw err;
    }
}

SQLLIB_SQLITE3_NS_END

#endif // NO_SQLITE3

SQLLIB_NS_END
