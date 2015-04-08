#ifndef SQLLIB_TEMPLATES_HPP
#define SQLLIB_TEMPLATES_HPP

#include <string>
#include <sstream>
#include <tuple>
#include <vector>

struct NoKey {};

template<int... IDs>
struct PrimaryKey {};

// Conditionals
template<bool, typename, typename>
struct IfElse;

template<typename Then, typename Else>
struct IfElse<true, Then, Else> {
    using result = Then;
};

template<typename Then, typename Else>
struct IfElse<false, Then, Else> {
    using result = Else;
};

template<bool, typename>
struct If;

template<typename Then>
struct If<true, Then> {
    using result = Then;
};

// Check ID Exists
template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value - 1>
struct FieldTupleIDExistsImpl {
    static constexpr bool value = (ID == std::tuple_element<N, Tuple>::type::getN()) || FieldTupleIDExistsImpl<Tuple, ID, N - 1>::value;
};

template<typename Tuple, int ID>
struct FieldTupleIDExistsImpl<Tuple, ID, 0> {
    static constexpr bool value = ID == std::tuple_element<0, Tuple>::type::getN();
};

template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value, bool GZ = (N > 0)>
struct FieldTupleIDExists; 

template<typename Tuple, int ID, int N>
struct FieldTupleIDExists<Tuple, ID, N, true> {
    static constexpr bool value = FieldTupleIDExistsImpl<Tuple, ID>::value;
};

template<typename Tuple, int ID, int N>
struct FieldTupleIDExists<Tuple, ID, N, false> {
    static constexpr bool value = false;
};

// Get Element From Tuple
template<typename Tuple, int N>
struct FieldTupleGetN {
    static const typename std::tuple_element<N, Tuple>::type& get(Tuple& tuple) {
        return std::get<N>(tuple);
    }
};

template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value - 1>
struct FieldTupleGetImpl : public IfElse<ID == std::tuple_element<N, Tuple>::type::getN(), FieldTupleGetN<Tuple, N>, FieldTupleGetImpl<Tuple, ID, N - 1>>::result {};

template<typename Tuple, int ID>
struct FieldTupleGetImpl<Tuple, ID, 0> : public If<ID == std::tuple_element<0, Tuple>::type::getN(), FieldTupleGetN<Tuple, 0>>::result {};

template<typename Tuple, int ID>
struct FieldTupleGet : public FieldTupleGetImpl<Tuple, ID> {};


template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value - 1>
struct FieldTupleGetIndexImpl {
    static const int value = IfElse<ID == std::tuple_element<N, Tuple>::type::getN(), std::integral_constant<int, N>, FieldTupleGetIndexImpl<Tuple, ID, N - 1>>::result::value;
};

template<typename Tuple, int ID>
struct FieldTupleGetIndexImpl<Tuple, ID, 0> {
    static const int value = If<ID == std::tuple_element<0, Tuple>::type::getN(), std::integral_constant<int, 0>>::result::value;
};

template<typename Tuple, int ID>
struct FieldTupleGetIndex {
    static const int value = FieldTupleGetIndexImpl<Tuple, ID>::value;
};

// Make Field Tuple
template<typename Tuple, int ID, int... IDs>
struct MakeFieldTuple {
    static auto make(Tuple& tuple) {
        auto tmpTuple = std::make_tuple(FieldTupleGet<Tuple, ID>::get(tuple));
        return std::tuple_cat(tmpTuple, MakeFieldTuple<Tuple, IDs...>::make(tuple));
    }
};

template<typename Tuple, int ID>
struct MakeFieldTuple<Tuple, ID> {
    static auto make(Tuple& tuple) {
        return std::make_tuple(FieldTupleGet<Tuple, ID>::get(tuple));
    }
};

// Make Type Tuple
template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct MakeTypeTuple {
    static auto make(Tuple& tuple) {
        auto tmpTuple = std::make_tuple(typename std::remove_reference<decltype(std::get<N>(tuple))>::type::Type());
        return std::tuple_cat(MakeTypeTuple<Tuple, N - 1>::make(tuple), tmpTuple);
    }
};
                        
template<typename Tuple>
struct MakeTypeTuple<Tuple, 0> {
    static auto make(Tuple& tuple) {
        return std::make_tuple(typename std::remove_reference<decltype(std::get<0>(tuple))>::type::Type());
    }
};

// Stringify
template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct FieldTupleStringer {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << FieldTupleStringer<Tuple, N - 1>::string(tuple);
        str << ", " << std::get<N>(tuple).getName() << " " << std::remove_reference<decltype(std::get<N>(tuple))>::type::Type::sqlType();
    
        return str.str();
    }
};

template<typename Tuple>
struct FieldTupleStringer<Tuple, 0> {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << std::get<0>(tuple).getName() << " " << std::remove_reference<decltype(std::get<0>(tuple))>::type::Type::sqlType();

        return str.str();
    }
};

template<typename Tuple, int N = std::tuple_size<Tuple>::value - 1>
struct FieldTupleNames {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << FieldTupleNames<Tuple, N - 1>::string(tuple);
        str << ", " << std::get<N>(tuple).getName();
    
        return str.str();
    }
};

template<typename Tuple>
struct FieldTupleNames<Tuple, 0> {
    static std::string string(Tuple& tuple) {
        std::ostringstream str;
        str << std::get<0>(tuple).getName();

        return str.str();
    }
};

// Primary Key Stringify
template<typename, typename>
struct PrimaryKeyStringer;

template<typename Tuple>
struct PrimaryKeyStringer<Tuple, NoKey> {
    static std::string string(Tuple& tuple) {
        return "";
    }
};

template<typename Tuple, int... IDs>
struct PrimaryKeyStringer<Tuple, PrimaryKey<IDs...>> {
    static std::string string(Tuple& tuple) {
        auto keyFields = MakeFieldTuple<Tuple, IDs...>::make(tuple);
        
        std::ostringstream str;
        str << ", PRIMARY KEY (";
        str << FieldTupleNames<decltype(keyFields)>::string(keyFields);
        str << ")";

        return str.str();
    }
};

#include "db.hpp"

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
