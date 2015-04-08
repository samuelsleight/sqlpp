#include "src/inc/db.hpp"
#include "src/inc/table.hpp"
#include "src/inc/templates.hpp"
#include "src/inc/string.hpp"
#include "src/inc/int.hpp"

#include <iostream>
#include <typeinfo>

int main(int argc, char* argv[]) {
    sqlpp::DB::Ptr db = sqlpp::DB::sqlite3("test.sql");

    if(db == nullptr) {
        std::cout << "ERROR D:" << std::endl;
    } else {
        std::cout << "YAY!" << std::endl;
    }

    auto tbl = db->addTable("test")
        ->addField<sqlpp::String<>, 1>("name")
        ->addField<sqlpp::Integer, 2>("age")
        ->setPrimaryKey<1, 2>()
        ->create();

    tbl->insert<2, 1>()
        ->values(20, "sam")
        ->values(17, "lemon")
        ->execute();

    auto rows = tbl->select()->execute();
    while(rows.next()) {
        std::cout << rows.get<1>() << ": " << rows.get<2>() << std::endl;
    }
}
