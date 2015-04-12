#ifndef SQLLIB_QUERY_HPP
#define SQLLIB_QUERY_HPP

#include "defines.hpp"
#include "connection.hpp"

SQLLIB_NS

class Query {
public:
    virtual ~Query();

    virtual std::string sql() = 0;
    virtual void execute(std::shared_ptr<Connection> connection) = 0;
};

SQLLIB_NS_END

#endif

