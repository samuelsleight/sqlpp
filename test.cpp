#include "src/db.hpp"
#include "src/connection.hpp"

#include <exception>

int main(int argc, char* argv[]) {
    auto db = sqlpp::database()
        ->addTable<0>("people")
        ->addField<0, int>("id")
        ->addField<1, std::string>("name")

        ->addTable<1>("cakes")
        ->addField<0, int>("id")
        ->addField<1, int>("pid");

    try {
        auto conn = sqlpp::Connection::sqlite3("test.sql");
        db->tables<0, 1>()->create(false)->execute(conn);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    db->tables<0>()->insert();
}
