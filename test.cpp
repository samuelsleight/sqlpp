#include "src/db.hpp"
#include "src/type.hpp"
#include "src/connection.hpp"

#include <exception>

int main(int argc, char* argv[]) {
    auto db = sqlpp::database()
        ->addTable<0>("people")
        ->addField<0, sqlpp::Integer>("id")
        ->addField<1, sqlpp::String<20>>("name")

        ->addTable<1>("cakes")
        ->addField<0, sqlpp::Integer>("id")
        ->addField<1, sqlpp::Integer>("pid");

    auto ins = db->tables<0>()
        ->insert<0, 1>()
        ->values(0, "sam")
        ->values(1, "alice");

    try {
        auto conn = sqlpp::Connection::sqlite3("test.sql");
        db->tables<0, 1>()->create()->execute(conn);
        ins->execute(conn);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
