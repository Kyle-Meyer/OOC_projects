#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <map>
#include <string>
#include <ostream>

class Node;
using node_ptr         = std::shared_ptr<Node>;
using SymbolTable      = std::map<std::string, double>;
using symbol_table_ptr = std::shared_ptr<SymbolTable>;

class Node
{
public:
    virtual ~Node() = default;
    virtual double   evaluate()                          const = 0;
    virtual node_ptr derivative(const std::string& var)  const = 0;
    virtual void     print(std::ostream& os)             const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const node_ptr& n)
{
    n->print(os);
    return os;
}

#endif
