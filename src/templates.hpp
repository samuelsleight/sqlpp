#ifndef SQLLIB_TEMPLATES_HPP
#define SQLLIB_TEMPLATES_HPP

#include "defines.hpp"

SQLLIB_NS

template<typename Tuple, typename T, typename F>
T tupleFold(Tuple& tuple, T acc, F func);

SQLLIB_NS_END

#include "tpp/templates.tpp"

#endif
