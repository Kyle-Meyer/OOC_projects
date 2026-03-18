#include <iostream>
#include <iomanip>
#include "Node.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Add.hpp"
#include "Sub.hpp"
#include "Mul.hpp"
#include "Div.hpp"
#include "UnaryMinus.hpp"

int main()
{
    // 1. Create shared symbol table and populate it
    auto table = std::make_shared<SymbolTable>();
    (*table)["Xray"]   = 2.0;
    (*table)["Yellow"] = 3.0;
    (*table)["Zebra"]  = 5.0;

    // 2. Convenience lambdas
    auto C = [](double v)              { return std::make_shared<Constant>(v); };
    auto V = [&](const std::string& n) { return std::make_shared<Variable>(n, table); };

    // 3. Construct the expression tree:
    //    2.3*Xray + Yellow*(Zebra - Xray)
    node_ptr tree = std::make_shared<Add>(
        std::make_shared<Mul>(C(2.3), V("Xray")),
        std::make_shared<Mul>(V("Yellow"),
            std::make_shared<Sub>(V("Zebra"), V("Xray")))
    );

    // 4. Print the tree
    std::cout << "Expression:\n  " << tree << "\n\n";

    // 5. Evaluate
    double val = tree->evaluate();
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "evaluate() = " << val << "  (expected 13.600000)\n\n";

    // 6. Compute derivative w.r.t. "Xray"
    node_ptr dx = tree->derivative("Xray");

    // 7. Print and evaluate the derivative
    std::cout << "Derivative w.r.t. Xray:\n  " << dx << "\n\n";
    double dval = dx->evaluate();
    std::cout << "derivative(\"Xray\").evaluate() = " << dval << "  (expected -0.700000)\n";

    return 0;
}
