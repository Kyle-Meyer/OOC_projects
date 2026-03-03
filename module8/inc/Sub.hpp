#ifndef SUB_HPP
#define SUB_HPP

#include "BinaryOp.hpp"

class Sub : public BinaryOp
{
public:
    Sub(node_ptr l, node_ptr r) : BinaryOp(l, r) {}

    double evaluate() const override
    {
        return left_->evaluate() - right_->evaluate();
    }

    node_ptr derivative(const std::string& var) const override
    {
        return std::make_shared<Sub>(
            left_->derivative(var),
            right_->derivative(var));
    }

    void print(std::ostream& os) const override
    {
        os << '(';
        left_->print(os);
        os << '-';
        right_->print(os);
        os << ')';
    }
};

#endif
