#ifndef SQLLIB_FIELD_HPP
#define SQLLIB_FIELD_HPP

#include "defines.hpp"

SQLLIB_NS

template<int, typename...>
class Table;

template<int ID, typename Type>
class Field {
public:
    Field() = default;

    static constexpr int getID();
    std::string getName();

    std::string getDefinition();

private:
    template<int, typename...>
    friend class Table;

    std::string name;
    static constexpr int id = ID;

    Field(std::string name);
};

SQLLIB_NS_END

#include "tpp/field.tpp"

#endif
