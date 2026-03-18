#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "Node.hpp"

class Constant : public Node
{
public:
    explicit Constant(double value) : value_(value) {}

    double evaluate() const override
    {
        return value_;
    }

    node_ptr derivative(const std::string& /*var*/) const override
    {
        return std::make_shared<Constant>(0.0);
    }

    void print(std::ostream& os) const override
    {
        os << '(' << value_ << ')';
    }

private:
    double value_;
};

#endif
