#include "inc/db.hpp"
#include "inc/table.hpp"

SQLLIB_NS

DB::~DB() {}

#ifndef NO_SQLITE3

DB::Ptr DB::sqlite3(std::string filename) {
    return SQLLIB_SQLITE3_NS_(DB)::connect(filename);
}

#endif

Table<std::tuple<>>::Ptr DB::addTable(std::string name) {
    return Table<std::tuple<>>::Ptr(new Table<std::tuple<>>(this, name));
}

#ifndef NO_SQLITE3

SQLLIB_SQLITE3_NS

DB::DB() {}

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
    int err = ::sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

    if(err != SQLITE_OK) {
        throw err;
    }
}

void DB::beginTransaction() {
    int err = ::sqlite3_exec(db, "BEGIN", nullptr, nullptr, nullptr);

    if(err != SQLITE_OK) {
        throw err;
    }
}

void DB::prepareQuery(std::string sql) {
    int err = ::sqlite3_prepare(db, sql.c_str(), -1, &stmt, nullptr);

    if(err != SQLITE_OK) {
        throw err;
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
    int err = ::sqlite3_step(stmt);
    
    if(err != SQLITE_DONE) {
        throw err;
    }

    ::sqlite3_reset(stmt);
}

void DB::cleanPreparedQuery() {
    ::sqlite3_finalize(stmt);
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
