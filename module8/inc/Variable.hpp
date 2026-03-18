#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "Node.hpp"
#include "Constant.hpp"

class Variable : public Node
{
public:
    Variable(const std::string& name, symbol_table_ptr table)
        : name_(name), table_(table) {}

    double evaluate() const override
    {
        return (*table_)[name_];
    }

    node_ptr derivative(const std::string& var) const override
    {
        if (name_ == var)
            return std::make_shared<Constant>(1.0);
        return std::make_shared<Constant>(0.0);
    }

    void print(std::ostream& os) const override
    {
        os << '(' << name_ << ')';
    }

private:
    std::string        name_;
    symbol_table_ptr   table_;
};

#endif
