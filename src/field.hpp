#ifndef SQLLIB_FIELD_HPP
#define SQLLIB_FIELD_HPP

#include "defines.hpp"
#include "statement.hpp"

#include <memory>

SQLLIB_NS

template<int, typename...>
class Table;

template<typename, int...>
class SelectImpl;

template<int ID, typename Type>
class Field {
public:
    using ValueType = Type;

    Field() = default;

    static constexpr int getID();
    std::string getName();

    std::string getDefinition();

private:
    template<int, typename...>
    friend class Table;

    template<typename, int...>
    friend class SelectImpl;

    std::string name;
    static constexpr int id = ID;

    Field(std::string name);

    static auto getValue(std::shared_ptr<Statement> conn, int index);
};

SQLLIB_NS_END

#include "tpp/field.tpp"

#endif
