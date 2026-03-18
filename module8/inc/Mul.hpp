#ifndef MUL_HPP
#define MUL_HPP

#include "BinaryOp.hpp"
#include "Add.hpp"

class Mul : public BinaryOp
{
public:
    Mul(node_ptr l, node_ptr r) : BinaryOp(l, r) {}

    double evaluate() const override
    {
        return left_->evaluate() * right_->evaluate();
    }

    // Product rule: d(u*v) = u*dv + v*du
    node_ptr derivative(const std::string& var) const override
    {
        return std::make_shared<Add>(
            std::make_shared<Mul>(left_,  right_->derivative(var)),
            std::make_shared<Mul>(right_, left_->derivative(var)));
    }

    void print(std::ostream& os) const override
    {
        os << '(';
        left_->print(os);
        os << '*';
        right_->print(os);
        os << ')';
    }
};

#endif
