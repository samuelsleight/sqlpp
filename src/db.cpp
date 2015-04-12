#include "db.hpp"

SQLLIB_NS

EmptyDB::EmptyDB() {}

EmptyDB::Ptr database() {
    return EmptyDB::Ptr(new EmptyDB());
}

SQLLIB_NS_END
