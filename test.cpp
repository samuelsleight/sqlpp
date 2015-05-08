#include "src/db.hpp"
#include "src/type.hpp"
#include "src/where.hpp"
#include "src/connection.hpp"

#include <exception>

int main(int argc, char* argv[]) {
    auto db = sqlpp::database()
        ->addTable<0>("people")
        ->addField<0, sqlpp::Integer>("id")
        ->addField<1, sqlpp::String<20>>("name")

        ->addTable<1>("cakes")
        ->addField<0, sqlpp::Integer>("id")
        ->addField<1, sqlpp::Integer>("baker")
        ->addField<2, sqlpp::Integer>("eatenby");

    auto people = db->tables<0>()
        ->insert<0, 1>()
        ->values(0, "sam")
        ->values(1, "alice")
        ->values(2, "bob")
        ->values(3, "charlie");

    auto cakes = db->tables<1>()
        ->insert<0, 1, 2>()
        ->values(0, 2, 0)
        ->values(1, 2, 0)
        ->values(2, 2, 3)
        ->values(3, 3, 2)
        ->values(4, 3, 1)
        ->values(5, 0, 1);

    auto sel = db->tables<1, 0, 0>()
		->select<0, 0>()
        ->select<1, 1>()
        ->select<2, 1>()
        ->where<sqlpp::And<sqlpp::Eq<0, 1, 1, 0>, sqlpp::Eq<0, 2, 2, 0>>>()
        ->callback([](int id, std::string baker, std::string eater) {
            std::cout << id << ": " << baker << " baked a cake for " << eater << std::endl;
        });

    std::cout << sel->sql() << std::endl;

    try {
        auto conn = sqlpp::Connection::sqlite3("test.sql");
        db->tables<0, 1>()->create()->execute(conn);
        people->execute(conn);
        cakes->execute(conn);
        sel->execute(conn);
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
