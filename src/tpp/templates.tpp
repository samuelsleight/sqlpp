#ifndef SQLLIB_TEMPLATES_TPP
#define SQLLIB_TEMPLATES_TPP

template<int...>
struct List {};

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

// IndexFromID
template<typename Tuple, int ID, int N = std::tuple_size<Tuple>::value>
struct IndexFromID {
    using ElementType = typename std::tuple_element<N - 1, Tuple>::type;

    static constexpr int value = IfElse<ID == ElementType::getID(), 
        std::integral_constant<int, N - 1>, 
        IndexFromID<Tuple, ID, N - 1>>::result::value;
};

template<typename Tuple, int ID>
struct IndexFromID<Tuple, ID, 1> {
    using ElementType = typename std::tuple_element<0, Tuple>::type;

    static constexpr int value = If<ID == ElementType::getID(), 
        std::integral_constant<int, 0>>::result::value;
};

// TupleFromIDs
template<typename Tuple, typename IDList, int... Indexes>
struct TupleFromIDs;

template<typename Tuple, int ID, int... IDs, int... Indexes>
struct TupleFromIDs<Tuple, List<ID, IDs...>, Indexes...> 
    : public TupleFromIDs<Tuple, List<IDs...>, Indexes..., IndexFromID<Tuple, ID>::value> {};

template<typename Tuple, int... Indexes>
struct TupleFromIDs<Tuple, List<>, Indexes...> {
    static auto make(Tuple& tableTuple) {
        return std::make_tuple(std::get<Indexes>(tableTuple)...);
    }
};

// ReplaceIndex
template<typename Tuple, typename T, int Index, int N = std::tuple_size<Tuple>::value, bool Replace = Index == N - 1>
struct ReplaceIndex;

template<typename Tuple, typename T, int Index, int N>
struct ReplaceIndex<Tuple, T, Index, N, true> {
    static auto replace(Tuple& t, T thing) {
        return std::tuple_cat(ReplaceIndex<Tuple, T, Index, N - 1>::replace(t, thing), std::make_tuple(thing));
    }
};

template<typename Tuple, typename T, int Index, int N>
struct ReplaceIndex<Tuple, T, Index, N, false> {
    static auto replace(Tuple& t, T thing) {
        auto element = std::get<N - 1>(t);
        return std::tuple_cat(ReplaceIndex<Tuple, T, Index, N - 1>::replace(t, thing), std::make_tuple(element));
    }
};

template<typename Tuple, typename T, int Index>
struct ReplaceIndex<Tuple, T, Index, 1, true> {
    static auto replace(Tuple& t, T thing) {
        return std::make_tuple(thing);
    }
};

template<typename Tuple, typename T, int Index>
struct ReplaceIndex<Tuple, T, Index, 1, false> {
    static auto replace(Tuple& t, T thing) {
        return std::make_tuple(std::get<0>(t));
    }
};

// TupleFold
template<typename Tuple, typename T, typename F, int N = std::tuple_size<Tuple>::value>
struct TupleFold {
    static T fold(Tuple& tuple, T acc, F func) {
        return TupleFold<Tuple, T, F, N - 1>::fold(tuple, func(acc, std::get<N - 1>(tuple)), func);
    }
};

template<typename Tuple, typename T, typename F>
struct TupleFold<Tuple, T, F, 1> {
    static T fold(Tuple& tuple, T acc, F func) {
        return func(acc, std::get<0>(tuple));
    }
};

// TupleForEach
template<typename Tuple, typename F, int N = std::tuple_size<Tuple>::value>
struct TupleForEach {
    static void foreach(Tuple& tuple, F func) {
        func(std::get<N - 1>(tuple));
        TupleForEach<Tuple, F, N - 1>::foreach(tuple, func);
    }
};

template<typename Tuple, typename F>
struct TupleForEach<Tuple, F, 1> {
    static void foreach(Tuple& tuple, F func) {
        func(std::get<0>(tuple));
    }
};

// ValueTypeOf
template<typename T>
struct ValueTypeOf {
    using Type = typename T::ValueType;
};

// SelectFields
template<typename TableTuple, int... Ns>
struct SelectFields;

template<typename TableTuple, int Index, int Field, int... Ns>
struct SelectFields<TableTuple, Index, Field, Ns...> {
    static std::string write(TableTuple& tuple) {
        std::ostringstream str;

        str << "t" << Index << ".";
        str << std::get<Index>(tuple).template field<Field>().getName();
        str << ", ";
        str << SelectFields<TableTuple, Ns...>::write(tuple);

        return str.str();
    }
};

template<typename TableTuple, int Index, int Field>
struct SelectFields<TableTuple, Index, Field> {
    static std::string write(TableTuple& tuple) {
        std::ostringstream str;

        str << "t" << Index << ".";
        str << std::get<Index>(tuple).template field<Field>().getName();

        return str.str();
    }
};

// WriteWhere
template<typename, typename>
struct WriteWhere;

// WriteWhere<And>
template<typename TableTuple, typename T, typename... Ts>
struct WriteWhere<TableTuple, SQLLIB_NS_(And)<T, Ts...>> {
    static std::string write(TableTuple& tuple) {
        std::ostringstream str;

        str << "(";
        str << WriteWhere<TableTuple, T>::write(tuple);
        str << ") AND ";
        str << WriteWhere<TableTuple, SQLLIB_NS_(And)<Ts...>>::write(tuple);

        return str.str();
    }
};

template<typename TableTuple, typename T>
struct WriteWhere<TableTuple, SQLLIB_NS_(And)<T>> {
    static std::string write(TableTuple& tuple) {
        std::ostringstream str;

        str << "(";
        str << WriteWhere<TableTuple, T>::write(tuple);
        str << ")";

        return str.str();
    }
};

template<typename TableTuple, int I1, int F1, int I2, int F2>
struct WriteWhere<TableTuple, SQLLIB_NS_(Eq)<I1, F1, I2, F2>> {
    static std::string write(TableTuple& tuple) {
        std::ostringstream str;

        str << "t" << std::to_string(I1) << ".";
        str << std::get<I1>(tuple).template field<F1>().getName();
        str << " = t" << std::to_string(I2) << ".";
        str << std::get<I2>(tuple).template field<F2>().getName();

        return str.str();
    }
};

SQLLIB_NS

template<typename Tuple, typename T, typename F>
T tupleFold(Tuple& tuple, T acc, F func) {
    return TupleFold<Tuple, T, F>::fold(tuple, acc, func);
}

template<typename Tuple, typename F>
void tupleForEach(Tuple& tuple, F func) {
    TupleForEach<Tuple, F>::foreach(tuple, func);
}

SQLLIB_NS_END

#endif
