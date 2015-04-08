#ifndef SQLLIB_RESULTS_HPP
#define SQLLIB_RESULTS_HPP

#include "defines.hpp"

SQLLIB_NS

template<typename>
class Select;

template<typename FieldTuple>
class SelectResult {
public:
    bool next();

    ~SelectResult() {
        close();
    }

    template<int ID>
    auto get() {
        return std::remove_reference<decltype(FieldTupleGet<FieldTuple, ID>::get(fields))>::type::Type::getValue(db, FieldTupleGetIndex<FieldTuple, ID>::value);
    }

    void close();

private:
    friend class Select<FieldTuple>;

    DB* db;
    FieldTuple fields;

    bool closed;

    SelectResult(DB* db, FieldTuple fields)
        : db(db), fields(fields), closed(false) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple>
bool SelectResult<FieldTuple>::next() {
    return db->selectNextRow();
}

template<typename FieldTuple>
void SelectResult<FieldTuple>::close() {
    if(!closed) {
        db->cleanPreparedQuery();
        closed = true;
    }
}

SQLLIB_NS_END

#endif
