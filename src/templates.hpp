#ifndef SQLLIB_TEMPLATES_HPP
#define SQLLIB_TEMPLATES_HPP

#include "defines.hpp"
#include "where.hpp"

#include <tuple>
#include <string>
#include <sstream>
#include <functional>

SQLLIB_NS

template<typename Tuple, typename T, typename F>
auto tupleFold(Tuple& tuple, T acc, F func);

template<typename Tuple, typename F>
void tupleForEach(Tuple& tuple, F func);

SQLLIB_NS_END

#include "tpp/templates.tpp"

#endif
