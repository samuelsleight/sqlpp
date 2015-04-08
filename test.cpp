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

    auto people = db->addTable<0>("people")
        ->addField<sqlpp::Integer, 0>("id")
        ->addField<sqlpp::String<10>, 1>("name")
        ->addField<sqlpp::Integer, 2>("age")
        ->setPrimaryKey<1>()
        ->create();

    auto cakes = db->addTable<1>("cakes")
        ->addField<sqlpp::Integer, 0>("id")
        ->addField<sqlpp::Integer, 1>("pid")
        ->addField<sqlpp::String<10>, 2>("size")
        ->setPrimaryKey<0>()
        ->addForeignKey<1>(people->fields<0>())
        ->create();

    people->insert<0, 1, 2>()
        ->values(0, "sam", 20)
        ->values(1, "bob", 46)
        ->execute();

    cakes->insert()
        ->values(0, 0, "big")
        ->values(1, 0, "big")
        ->values(2, 0, "small")
        ->values(3, 1, "medium")
        ->execute();
}
