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
        ->addRow<sqlpp::String<>>("name")
        ->addRow<sqlpp::Integer>("age")
        ->create();

    for(std::string row : tbl->getRowNames()) {
        std::cout << row << std::endl;
    }
}
