#ifndef SQLLIB_TEMPLATES_HPP
#define SQLLIB_TEMPLATES_HPP

#include <string>
#include <vector>

struct Nil {};

template<typename T, typename L>
struct Cons {
    using Head = T;
    using Tail = L;
};

template<typename List>
struct ParseTypeList {
    static std::vector<std::string> parse() {
        std::vector<std::string> list = { List::Head::sqlType() };
        std::vector<std::string> tail = ParseTypeList<typename List::Tail>::parse();
        list.insert(list.end(), tail.begin(), tail.end());

        return list;
    }
};

template<>
struct ParseTypeList<Nil> {
    static std::vector<std::string> parse() {
        return std::vector<std::string>();
    }
};

#endif
