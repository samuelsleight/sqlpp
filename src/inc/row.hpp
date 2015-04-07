#ifndef SQLLIB_ROW_HPP
#define SQLLIB_ROW_HPP

#include "defines.hpp"
#include "templates.hpp"
#include "db.hpp"

#include <tuple>
#include <type_traits>

SQLLIB_NS

template<typename T, int N>
class Row {
private:
    std::string name;
    static constexpr int rowid = N;

public:
    using Type = T;

    Row(std::string name)
        : name(name) {}

    std::string getName() const {
        return name;
    }

    static constexpr int getN() {
        return rowid;
    }
};

SQLLIB_NS_END

template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value - 1>
struct RowTupleIDExistsImpl {
    static constexpr bool value = (ID == std::tuple_element<N, Tuple>::type::getN()) || RowTupleIDExistsImpl<Tuple, ID, N - 1>::value;
};

template<typename Tuple, int ID>
struct RowTupleIDExistsImpl<Tuple, ID, 0> {
    static constexpr bool value = ID == std::tuple_element<0, Tuple>::type::getN();
};

template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value, bool GZ = (N > 0)>
struct RowTupleIDExists; 

template<typename Tuple, int ID, int N>
struct RowTupleIDExists<Tuple, ID, N, true> {
    static constexpr bool value = RowTupleIDExistsImpl<Tuple, ID>::value;
};

template<typename Tuple, int ID, int N>
struct RowTupleIDExists<Tuple, ID, N, false> {
    static constexpr bool value = false;
};

template<typename Tuple, int N>
struct RowTupleGetN {
    static const typename std::tuple_element<N, Tuple>::type& get(Tuple& tuple) {
        return std::get<N>(tuple);
    }
};

template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value - 1>
struct RowTupleGetImpl : public IfElse<ID == std::tuple_element<N, Tuple>::type::getN(), RowTupleGetN<Tuple, N>, RowTupleGetImpl<Tuple, ID, N - 1>>::result {};

template<typename Tuple, int ID>
struct RowTupleGetImpl<Tuple, ID, 0> : public If<ID == std::tuple_element<0, Tuple>::type::getN(), RowTupleGetN<Tuple, 0>>::result {};

template<typename Tuple, int ID>
struct RowTupleGet : public RowTupleGetImpl<Tuple, ID> {};

// Stringify
template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct RowTupleStringer {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << RowTupleStringer<Tuple, N - 1>::string(tuple);
        str << ", " << std::get<N>(tuple).getName() << " " << std::remove_reference<decltype(std::get<N>(tuple))>::type::Type::sqlType();
    
        return str.str();
    }
};

template<typename Tuple>
struct RowTupleStringer<Tuple, 0> {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << std::get<0>(tuple).getName() << " " << std::remove_reference<decltype(std::get<0>(tuple))>::type::Type::sqlType();

        return str.str();
    }
};

template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct RowTupleNames {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << RowTupleNames<Tuple, N - 1>::string(tuple);
        str << ", " << std::get<N>(tuple).getName();
    
        return str.str();
    }
};

template<typename Tuple>
struct RowTupleNames<Tuple, 0> {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << std::get<0>(tuple).getName();

        return str.str();
    }
};

// Bind
template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct ValueTupleBind {
    static void bind(SQLLIB_NS_(DB)* db, const Tuple& tuple) {
        std::get<N>(tuple).bind(db, N + 1);
        ValueTupleBind<Tuple, N - 1>::bind(db, tuple);
    }
};

template<typename Tuple>
struct ValueTupleBind<Tuple, 0> {
    static void bind(SQLLIB_NS_(DB)* db, const Tuple& tuple) {
        std::get<0>(tuple).bind(db, 1);
    }
};

#endif
