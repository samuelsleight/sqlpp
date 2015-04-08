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
    std::string val;

public:
    String() {}
    String(std::string val) : val(val) {}
    String(const char* val) : val(val) {}
    String(char* val) : val(val) {}

    static std::string sqlType() {
        std::ostringstream str;
        str << "VARCHAR(" << N << ")";
        return str.str();
    }

    static String<N> getValue(DB* db, int index) {
        return String<N>(db->selectStringValue(index));
    }

    std::string value() {
        return val;
    }

    void bind(DB* db, int index) const {
        db->bindString(index, val);
    }
};

SQLLIB_NS_END

#endif
