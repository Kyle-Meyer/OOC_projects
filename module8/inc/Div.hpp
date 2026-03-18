#ifndef DIV_HPP
#define DIV_HPP

#include "BinaryOp.hpp"
#include "Sub.hpp"
#include "Mul.hpp"

class Div : public BinaryOp
{
public:
    Div(node_ptr l, node_ptr r) : BinaryOp(l, r) {}

    double evaluate() const override
    {
        return left_->evaluate() / right_->evaluate();
    }

    // Quotient rule: d(u/v) = (v*du - u*dv) / (v*v)
    node_ptr derivative(const std::string& var) const override
    {
        return std::make_shared<Div>(
            std::make_shared<Sub>(
                std::make_shared<Mul>(right_, left_->derivative(var)),
                std::make_shared<Mul>(left_,  right_->derivative(var))),
            std::make_shared<Mul>(right_, right_));
    }

    void print(std::ostream& os) const override
    {
        os << '(';
        left_->print(os);
        os << '/';
        right_->print(os);
        os << ')';
    }
};

#endif
