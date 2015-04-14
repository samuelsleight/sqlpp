#ifndef SQLLIB_WHERE_HPP
#define SQLLIB_WHERE_HPP

#include "defines.hpp"

SQLLIB_NS

// Where Statements
template<int Index1, int FieldID1, int Index2, int FieldID2>
struct Eq;

template<typename...>
struct And;

SQLLIB_NS_END

#endif
