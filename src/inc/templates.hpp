#ifndef SQLLIB_TEMPLATES_HPP
#define SQLLIB_TEMPLATES_HPP

#include <string>
#include <tuple>
#include <vector>

struct Void {};

template<typename...> 
struct ListAdd;

template<template<typename...> class List, typename T>
struct ListAdd<List<Void>, T> {
    using list = List<T>;
};

template<template<typename...> class List, typename... Ts, typename T>
struct ListAdd<List<Ts...>, T> {
    using list = List<Ts..., T>;
};

template<typename...>
struct TypeList {};

template<>
struct TypeList<Void> {};

template<typename T, typename... Ts>
std::vector<std::string> parseTypeList(TypeList<T, Ts...>) {
    std::vector<std::string> list = { T::sqlType() };
    std::vector<std::string> tail = parseTypeList(TypeList<Ts...>());
    list.insert(list.end(), tail.begin(), tail.end());

    return list;
}

template<typename T>
std::vector<std::string> parseTypeList(TypeList<T>) {
    return { T::sqlType() };
}

#endif
