#ifndef UNARYMINUS_HPP
#define UNARYMINUS_HPP

#include "Node.hpp"

class UnaryMinus : public Node
{
public:
    explicit UnaryMinus(node_ptr child) : child_(child) {}

    double evaluate() const override
    {
        return -child_->evaluate();
    }

    node_ptr derivative(const std::string& var) const override
    {
        return std::make_shared<UnaryMinus>(child_->derivative(var));
    }

    void print(std::ostream& os) const override
    {
        os << "(-";
        child_->print(os);
        os << ')';
    }

private:
    node_ptr child_;
};

#endif
