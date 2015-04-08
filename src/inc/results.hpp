#ifndef SQLLIB_RESULTS_HPP
#define SQLLIB_RESULTS_HPP

#include "defines.hpp"
#include "templates.hpp"

SQLLIB_NS

template<typename, typename...>
class Select;

template<typename FieldTuple, typename... FieldTypes>
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

    std::tuple<FieldTypes...> getTuple() {
        return MakeResultTuple<std::tuple<FieldTypes...>>::make(db);
    }

    void close();

private:
    friend class Select<FieldTuple, FieldTypes...>;

    DB* db;
    FieldTuple fields;

    bool closed;

    SelectResult(DB* db, FieldTuple fields)
        : db(db), fields(fields), closed(false) {}
};

SQLLIB_NS_END

#include "db.hpp"

SQLLIB_NS

template<typename FieldTuple, typename... FieldTypes>
bool SelectResult<FieldTuple, FieldTypes...>::next() {
    return db->selectNextRow();
}

template<typename FieldTuple, typename... FieldTypes>
void SelectResult<FieldTuple, FieldTypes...>::close() {
    if(!closed) {
        db->cleanPreparedQuery();
        closed = true;
    }
}

SQLLIB_NS_END

#endif
