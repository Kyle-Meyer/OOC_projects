#ifndef BINARYOP_HPP
#define BINARYOP_HPP

#include "Node.hpp"

class BinaryOp : public Node
{
protected:
    node_ptr left_;
    node_ptr right_;

    BinaryOp(node_ptr l, node_ptr r) : left_(l), right_(r) {}
};

#endif
