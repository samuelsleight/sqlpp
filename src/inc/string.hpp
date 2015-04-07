#ifndef SQLLIB_STRING_HPP
#define SQLLIB_STRING_HPP

#include "defines.hpp"
#include "type.hpp"
#include "db.hpp"

#include <string>
#include <sstream>

SQLLIB_NS

template<int N = 255>
class String : public Type {
private:
    std::string value;

public:
    String() {}
    String(std::string value) : value(value) {}
    String(const char* value) : value(value) {}
    String(char* value) : value(value) {}

    static std::string sqlType() {
        std::ostringstream str;
        str << "VARCHAR(" << N << ")";
        return str.str();
    }

    void bind(DB* db, int index) const {
        db->bindString(index, value);
    }
};

SQLLIB_NS_END

#endif
