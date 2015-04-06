#include "inc/db.hpp"
#include "inc/table.hpp"

SQLLIB_NS

DB::~DB() {}

#ifndef NO_SQLITE3

DB::Ptr DB::sqlite3(std::string filename) {
    return SQLLIB_SQLITE3_NS_(DB)::connect(filename);
}

Table<TypeList<Void>, std::tuple<>>::Ptr DB::addTable(std::string name) {
    return Table<TypeList<Void>, std::tuple<>>::Ptr(new Table<TypeList<Void>, std::tuple<>>(this, name));
}

#endif

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

SQLLIB_SQLITE3_NS_END

#endif // NO_SQLITE3

SQLLIB_NS_END
