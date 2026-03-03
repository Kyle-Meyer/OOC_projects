// Exhaustive test suite for the expression-tree class hierarchy.
// Compiled as a standalone executable; no external test framework required.

#include <iostream>
#include <sstream>
#include <cmath>
#include <cassert>
#include <string>

#include "Node.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "Add.hpp"
#include "Sub.hpp"
#include "Mul.hpp"
#include "Div.hpp"
#include "UnaryMinus.hpp"

// ---------------------------------------------------------------------------
// Minimal test harness
// ---------------------------------------------------------------------------
static int g_total  = 0;
static int g_passed = 0;

static void check(bool condition, const std::string& label)
{
    ++g_total;
    if (condition) {
        ++g_passed;
        std::cout << "  [PASS] " << label << "\n";
    } else {
        std::cout << "  [FAIL] " << label << "\n";
    }
}

static void check_near(double actual, double expected, const std::string& label,
                       double tol = 1e-9)
{
    check(std::fabs(actual - expected) < tol, label);
}

static std::string print_str(const node_ptr& n)
{
    std::ostringstream oss;
    n->print(oss);
    return oss.str();
}

// ---------------------------------------------------------------------------
// Helper factories
// ---------------------------------------------------------------------------
static auto make_table()
{
    auto t = std::make_shared<SymbolTable>();
    (*t)["x"] = 2.0;
    (*t)["y"] = 3.0;
    (*t)["z"] = 5.0;
    return t;
}

// ===========================================================================
// 1. Constant
// ===========================================================================
static void test_constant()
{
    std::cout << "\n=== Constant ===\n";

    auto c0  = std::make_shared<Constant>(0.0);
    auto c5  = std::make_shared<Constant>(5.0);
    auto cneg = std::make_shared<Constant>(-3.7);

    // evaluate
    check_near(c0->evaluate(),   0.0,  "Constant(0) evaluate");
    check_near(c5->evaluate(),   5.0,  "Constant(5) evaluate");
    check_near(cneg->evaluate(), -3.7, "Constant(-3.7) evaluate");

    // derivative is always Constant(0)
    check_near(c5->derivative("x")->evaluate(), 0.0,
               "Constant derivative wrt x == 0");
    check_near(c5->derivative("y")->evaluate(), 0.0,
               "Constant derivative wrt y == 0");

    // print
    check(print_str(c5) == "(5)", "Constant(5) print == (5)");
    check(print_str(c0) == "(0)", "Constant(0) print == (0)");
    // negative value
    check(print_str(cneg) == "(-3.7)", "Constant(-3.7) print == (-3.7)");

    // operator<< via node_ptr
    node_ptr np = c5;
    std::ostringstream oss;
    oss << np;
    check(oss.str() == "(5)", "operator<< on Constant node_ptr");
}

// ===========================================================================
// 2. Variable
// ===========================================================================
static void test_variable()
{
    std::cout << "\n=== Variable ===\n";

    auto t = make_table();
    auto vx = std::make_shared<Variable>("x", t);
    auto vy = std::make_shared<Variable>("y", t);

    // evaluate — looks up from table
    check_near(vx->evaluate(), 2.0, "Variable x evaluate == 2.0");
    check_near(vy->evaluate(), 3.0, "Variable y evaluate == 3.0");

    // symbol-table mutation is immediately visible
    (*t)["x"] = 10.0;
    check_near(vx->evaluate(), 10.0, "Variable x after table mutation == 10.0");
    (*t)["x"] = 2.0; // restore

    // derivative wrt self == Constant(1)
    check_near(vx->derivative("x")->evaluate(), 1.0,
               "Variable x derivative wrt x == 1.0");
    check_near(vy->derivative("y")->evaluate(), 1.0,
               "Variable y derivative wrt y == 1.0");

    // derivative wrt other == Constant(0)
    check_near(vx->derivative("y")->evaluate(), 0.0,
               "Variable x derivative wrt y == 0.0");
    check_near(vy->derivative("x")->evaluate(), 0.0,
               "Variable y derivative wrt x == 0.0");
    check_near(vx->derivative("z")->evaluate(), 0.0,
               "Variable x derivative wrt z == 0.0");

    // print
    check(print_str(vx) == "(x)", "Variable x print == (x)");
    check(print_str(vy) == "(y)", "Variable y print == (y)");

    // shared table: two Variable nodes for same name share state
    auto vx2 = std::make_shared<Variable>("x", t);
    (*t)["x"] = 7.0;
    check_near(vx->evaluate(),  7.0, "Shared table: vx sees update");
    check_near(vx2->evaluate(), 7.0, "Shared table: vx2 sees update");
    (*t)["x"] = 2.0; // restore
}

// ===========================================================================
// 3. UnaryMinus
// ===========================================================================
static void test_unary_minus()
{
    std::cout << "\n=== UnaryMinus ===\n";

    auto t  = make_table();
    auto c5 = std::make_shared<Constant>(5.0);
    auto vx = std::make_shared<Variable>("x", t);

    auto neg5  = std::make_shared<UnaryMinus>(c5);
    auto negvx = std::make_shared<UnaryMinus>(vx);

    // evaluate
    check_near(neg5->evaluate(),  -5.0, "UnaryMinus(5) evaluate == -5");
    check_near(negvx->evaluate(), -2.0, "UnaryMinus(x) evaluate == -2");

    // derivative of -c == -0 == 0
    check_near(neg5->derivative("x")->evaluate(), 0.0,
               "UnaryMinus(5) derivative evaluate == 0");
    // derivative of -x == -1
    check_near(negvx->derivative("x")->evaluate(), -1.0,
               "UnaryMinus(x) derivative wrt x evaluate == -1");
    // derivative of -x wrt y == 0
    check_near(negvx->derivative("y")->evaluate(), 0.0,
               "UnaryMinus(x) derivative wrt y evaluate == 0");

    // print
    check(print_str(neg5)  == "(-(5))",  "UnaryMinus(5) print");
    check(print_str(negvx) == "(-(x))",  "UnaryMinus(x) print");

    // nested: -(-5) == 5
    auto double_neg = std::make_shared<UnaryMinus>(neg5);
    check_near(double_neg->evaluate(), 5.0, "-(-(5)) evaluate == 5");
    check(print_str(double_neg) == "(-(-(5)))", "-(-(5)) print");
}

// ===========================================================================
// 4. Add
// ===========================================================================
static void test_add()
{
    std::cout << "\n=== Add ===\n";

    auto t  = make_table();
    auto c2 = std::make_shared<Constant>(2.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto vx = std::make_shared<Variable>("x", t);
    auto vy = std::make_shared<Variable>("y", t);

    // constant + constant
    auto cc = std::make_shared<Add>(c2, c3);
    check_near(cc->evaluate(), 5.0, "Add(2,3) evaluate == 5");
    check(print_str(cc) == "((2)+(3))", "Add(2,3) print");
    check_near(cc->derivative("x")->evaluate(), 0.0, "Add(2,3) derivative == 0");

    // variable + variable: x + y = 5
    auto vv = std::make_shared<Add>(vx, vy);
    check_near(vv->evaluate(), 5.0, "Add(x,y) evaluate == 5");
    check(print_str(vv) == "((x)+(y))", "Add(x,y) print");
    check_near(vv->derivative("x")->evaluate(), 1.0,
               "Add(x,y) d/dx evaluate == 1");
    check_near(vv->derivative("y")->evaluate(), 1.0,
               "Add(x,y) d/dy evaluate == 1");
    check_near(vv->derivative("z")->evaluate(), 0.0,
               "Add(x,y) d/dz evaluate == 0");

    // variable + constant: x + 10 = 12; d/dx = 1
    auto vc = std::make_shared<Add>(vx, std::make_shared<Constant>(10.0));
    check_near(vc->evaluate(), 12.0, "Add(x,10) evaluate == 12");
    check_near(vc->derivative("x")->evaluate(), 1.0, "Add(x,10) d/dx == 1");

    // add with zero (identity)
    auto add_zero = std::make_shared<Add>(vx, std::make_shared<Constant>(0.0));
    check_near(add_zero->evaluate(), 2.0, "Add(x,0) evaluate == x");
}

// ===========================================================================
// 5. Sub
// ===========================================================================
static void test_sub()
{
    std::cout << "\n=== Sub ===\n";

    auto t  = make_table();
    auto c5 = std::make_shared<Constant>(5.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto vx = std::make_shared<Variable>("x", t);
    auto vy = std::make_shared<Variable>("y", t);

    // constant - constant
    auto cc = std::make_shared<Sub>(c5, c3);
    check_near(cc->evaluate(), 2.0, "Sub(5,3) evaluate == 2");
    check(print_str(cc) == "((5)-(3))", "Sub(5,3) print");
    check_near(cc->derivative("x")->evaluate(), 0.0, "Sub(5,3) derivative == 0");

    // variable - variable: x - y = -1
    auto vv = std::make_shared<Sub>(vx, vy);
    check_near(vv->evaluate(), -1.0, "Sub(x,y) evaluate == -1");
    check(print_str(vv) == "((x)-(y))", "Sub(x,y) print");
    check_near(vv->derivative("x")->evaluate(),  1.0, "Sub(x,y) d/dx == 1");
    check_near(vv->derivative("y")->evaluate(), -1.0, "Sub(x,y) d/dy == -1");
    check_near(vv->derivative("z")->evaluate(),  0.0, "Sub(x,y) d/dz == 0");

    // result negative
    auto neg = std::make_shared<Sub>(c3, c5);
    check_near(neg->evaluate(), -2.0, "Sub(3,5) evaluate == -2");
}

// ===========================================================================
// 6. Mul
// ===========================================================================
static void test_mul()
{
    std::cout << "\n=== Mul ===\n";

    auto t  = make_table();
    auto c2 = std::make_shared<Constant>(2.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto vx = std::make_shared<Variable>("x", t);
    auto vy = std::make_shared<Variable>("y", t);

    // constant * constant
    auto cc = std::make_shared<Mul>(c2, c3);
    check_near(cc->evaluate(), 6.0, "Mul(2,3) evaluate == 6");
    check(print_str(cc) == "((2)*(3))", "Mul(2,3) print");
    check_near(cc->derivative("x")->evaluate(), 0.0, "Mul(2,3) derivative == 0");

    // x * y: evaluate 2*3 == 6; d/dx == y == 3; d/dy == x == 2
    auto vv = std::make_shared<Mul>(vx, vy);
    check_near(vv->evaluate(), 6.0, "Mul(x,y) evaluate == 6");
    check(print_str(vv) == "((x)*(y))", "Mul(x,y) print");
    check_near(vv->derivative("x")->evaluate(), 3.0, "Mul(x,y) d/dx == y == 3");
    check_near(vv->derivative("y")->evaluate(), 2.0, "Mul(x,y) d/dy == x == 2");
    check_near(vv->derivative("z")->evaluate(), 0.0, "Mul(x,y) d/dz == 0");

    // 2*x: evaluate 4; d/dx == 2
    auto cx = std::make_shared<Mul>(c2, vx);
    check_near(cx->evaluate(), 4.0, "Mul(2,x) evaluate == 4");
    check_near(cx->derivative("x")->evaluate(), 2.0, "Mul(2,x) d/dx == 2");

    // multiply by zero
    auto zero = std::make_shared<Mul>(std::make_shared<Constant>(0.0), vx);
    check_near(zero->evaluate(), 0.0, "Mul(0,x) evaluate == 0");

    // x*x: evaluate 4; d/dx == x+x == 4  (product rule: x*1 + x*1)
    auto vx2 = std::make_shared<Variable>("x", t);
    auto xx = std::make_shared<Mul>(vx, vx2);
    check_near(xx->evaluate(), 4.0, "Mul(x,x) evaluate == x^2 == 4");
    check_near(xx->derivative("x")->evaluate(), 4.0, "Mul(x,x) d/dx == 2x == 4");
}

// ===========================================================================
// 7. Div
// ===========================================================================
static void test_div()
{
    std::cout << "\n=== Div ===\n";

    auto t  = make_table();
    auto c6 = std::make_shared<Constant>(6.0);
    auto c2 = std::make_shared<Constant>(2.0);
    auto c3 = std::make_shared<Constant>(3.0);
    auto vx = std::make_shared<Variable>("x", t);  // x=2
    auto vy = std::make_shared<Variable>("y", t);  // y=3

    // constant / constant
    auto cc = std::make_shared<Div>(c6, c2);
    check_near(cc->evaluate(), 3.0, "Div(6,2) evaluate == 3");
    check(print_str(cc) == "((6)/(2))", "Div(6,2) print");
    check_near(cc->derivative("x")->evaluate(), 0.0, "Div(6,2) derivative == 0");

    // x / y: x=2, y=3 → 2/3
    auto vv = std::make_shared<Div>(vx, vy);
    check_near(vv->evaluate(), 2.0 / 3.0, "Div(x,y) evaluate == 2/3");
    check(print_str(vv) == "((x)/(y))", "Div(x,y) print");

    // d/dx (x/y) = (y*1 - x*0) / (y*y) = y/y² = 1/y = 1/3
    check_near(vv->derivative("x")->evaluate(), 1.0 / 3.0,
               "Div(x,y) d/dx == 1/y == 1/3");

    // d/dy (x/y) = (y*0 - x*1) / (y*y) = -x/y² = -2/9
    check_near(vv->derivative("y")->evaluate(), -2.0 / 9.0,
               "Div(x,y) d/dy == -x/y² == -2/9");

    // d/dz (x/y) = 0
    check_near(vv->derivative("z")->evaluate(), 0.0,
               "Div(x,y) d/dz == 0");

    // division by 1
    auto div1 = std::make_shared<Div>(vx, std::make_shared<Constant>(1.0));
    check_near(div1->evaluate(), 2.0, "Div(x,1) evaluate == x == 2");

    // numerator zero
    auto zero_num = std::make_shared<Div>(std::make_shared<Constant>(0.0), vy);
    check_near(zero_num->evaluate(), 0.0, "Div(0,y) evaluate == 0");
}

// ===========================================================================
// 8. Composite / integration tests
// ===========================================================================
static void test_composite()
{
    std::cout << "\n=== Composite (assignment example) ===\n";

    // Reproduce the exact assignment tree:
    // 2.3*Xray + Yellow*(Zebra - Xray)
    // Xray=2, Yellow=3, Zebra=5 → 2.3*2 + 3*(5-2) = 4.6 + 9 = 13.6

    auto table = std::make_shared<SymbolTable>();
    (*table)["Xray"]   = 2.0;
    (*table)["Yellow"] = 3.0;
    (*table)["Zebra"]  = 5.0;

    auto C  = [](double v)              { return std::make_shared<Constant>(v); };
    auto V  = [&](const std::string& n) { return std::make_shared<Variable>(n, table); };

    node_ptr tree = std::make_shared<Add>(
        std::make_shared<Mul>(C(2.3), V("Xray")),
        std::make_shared<Mul>(V("Yellow"),
            std::make_shared<Sub>(V("Zebra"), V("Xray")))
    );

    check_near(tree->evaluate(), 13.6, "Assignment tree evaluate == 13.6");

    node_ptr dx = tree->derivative("Xray");
    check_near(dx->evaluate(), -0.7, "Assignment tree d/dXray evaluate == -0.7",
               1e-9);

    // derivative tree doesn't alter the original
    check_near(tree->evaluate(), 13.6,
               "Original tree unchanged after derivative()");

    // Mutate table; original tree reflects new value without rebuild
    (*table)["Xray"] = 0.0;
    // 2.3*0 + 3*(5-0) = 0 + 15 = 15
    check_near(tree->evaluate(), 15.0,
               "Tree re-evaluates after symbol table mutation (Xray=0)");
    (*table)["Xray"] = 2.0; // restore

    // Print round-trip: output must contain the expected structure characters
    std::string s = print_str(tree);
    check(!s.empty(), "Assignment tree print is non-empty");
    check(s.front() == '(' && s.back() == ')', "Assignment tree print wrapped in parens");
}

// ===========================================================================
// 9. Deeply nested / chain tests
// ===========================================================================
static void test_deep()
{
    std::cout << "\n=== Deep nesting ===\n";

    auto t  = make_table();
    auto vx = std::make_shared<Variable>("x", t); // x=2

    // x + (x + (x + x)) == 4x == 8
    auto inner = std::make_shared<Add>(vx, vx);
    auto mid   = std::make_shared<Add>(vx, inner);
    auto outer = std::make_shared<Add>(vx, mid);
    check_near(outer->evaluate(), 8.0, "x+(x+(x+x)) evaluate == 8");
    // d/dx = 1+1+1+1 = 4
    check_near(outer->derivative("x")->evaluate(), 4.0,
               "x+(x+(x+x)) d/dx == 4");

    // Nested Mul: x * (x * x) == x^3 == 8; d/dx = x*(x+x) + x^2 = 2x^2+x^2 = 3x^2 = 12
    auto vx2 = std::make_shared<Variable>("x", t);
    auto vx3 = std::make_shared<Variable>("x", t);
    auto x_sq  = std::make_shared<Mul>(vx2, vx3);
    auto x_cub = std::make_shared<Mul>(vx, x_sq);
    check_near(x_cub->evaluate(), 8.0, "x*(x*x) evaluate == x^3 == 8");
    check_near(x_cub->derivative("x")->evaluate(), 12.0,
               "x*(x*x) d/dx == 3x^2 == 12");

    // UnaryMinus chain
    auto t2  = make_table();
    auto vx4 = std::make_shared<Variable>("x", t2); // x=2
    auto neg  = std::make_shared<UnaryMinus>(vx4);
    auto neg2 = std::make_shared<UnaryMinus>(neg);
    check_near(neg->evaluate(),  -2.0, "-(x) evaluate == -2");
    check_near(neg2->evaluate(),  2.0, "-(-(x)) evaluate == 2");
    check_near(neg->derivative("x")->evaluate(),  -1.0, "-(x) d/dx == -1");
    check_near(neg2->derivative("x")->evaluate(),  1.0, "-(-(x)) d/dx == 1");
}

// ===========================================================================
// 10. Shared-pointer sharing between original and derivative trees
// ===========================================================================
static void test_shared_subtrees()
{
    std::cout << "\n=== Shared subtrees (Mul/Div derivative) ===\n";

    auto t  = make_table();
    auto vx = std::make_shared<Variable>("x", t); // x=2
    auto vy = std::make_shared<Variable>("y", t); // y=3

    // f(x,y) = x*y  →  df/dx = x*dy/dx + y*dx/dx = x*0 + y*1 = y = 3
    // The derivative tree holds shared pointers to the same vx/vy nodes.
    // Mutating the table still works after deriving.
    auto mul = std::make_shared<Mul>(vx, vy);
    auto dm  = mul->derivative("x");

    check_near(mul->evaluate(), 6.0, "Mul(x,y) before mutation == 6");
    check_near(dm->evaluate(),  3.0, "Mul(x,y) d/dx before mutation == 3");

    (*t)["x"] = 4.0;
    (*t)["y"] = 5.0;

    // original: 4*5 = 20
    check_near(mul->evaluate(), 20.0,
               "Mul(x,y) after mutation (x=4,y=5) == 20");
    // derivative (product rule applied, shared nodes): y=5
    check_near(dm->evaluate(), 5.0,
               "Mul(x,y) d/dx after mutation == y == 5");

    (*t)["x"] = 2.0;
    (*t)["y"] = 3.0;
}

// ===========================================================================
// 11. operator<< free function
// ===========================================================================
static void test_operator_stream()
{
    std::cout << "\n=== operator<< ===\n";

    auto c = std::make_shared<Constant>(42.0);
    node_ptr np = c;

    std::ostringstream oss;
    oss << np;
    check(oss.str() == "(42)", "operator<< Constant(42) == (42)");

    // Chain multiple outputs
    auto t   = make_table();
    auto vx  = std::make_shared<Variable>("x", t);
    node_ptr add = std::make_shared<Add>(c, vx);
    std::ostringstream oss2;
    oss2 << add;
    check(oss2.str() == "((42)+(x))", "operator<< Add(42,x)");
}

// ===========================================================================
// 12. Edge cases
// ===========================================================================
static void test_edge_cases()
{
    std::cout << "\n=== Edge cases ===\n";

    // Constant(0) derivative is Constant(0)
    auto c0 = std::make_shared<Constant>(0.0);
    check_near(c0->derivative("x")->evaluate(), 0.0, "Constant(0) derivative == 0");

    // Variable: derivative returns Constant, not Variable
    auto t  = make_table();
    auto vx = std::make_shared<Variable>("x", t);
    auto d  = vx->derivative("x"); // should be Constant(1)
    // Evaluating after table change should still return 1 (it's a Constant)
    (*t)["x"] = 999.0;
    check_near(d->evaluate(), 1.0,
               "Variable d/dx is Constant(1), unaffected by table changes");
    (*t)["x"] = 2.0;

    // Sub self: x - x == 0; d/dx = 1 - 1 = 0
    auto vx2 = std::make_shared<Variable>("x", t);
    auto self_sub = std::make_shared<Sub>(vx, vx2);
    check_near(self_sub->evaluate(), 0.0, "Sub(x,x) evaluate == 0");
    check_near(self_sub->derivative("x")->evaluate(), 0.0, "Sub(x,x) d/dx == 0");

    // Division of same var: x/x = 1; d/dx = (x*1 - x*1)/(x*x) = 0
    auto vx3 = std::make_shared<Variable>("x", t);
    auto self_div = std::make_shared<Div>(vx, vx3);
    check_near(self_div->evaluate(), 1.0, "Div(x,x) evaluate == 1");
    check_near(self_div->derivative("x")->evaluate(), 0.0, "Div(x,x) d/dx == 0");

    // Large constant
    auto big = std::make_shared<Constant>(1e15);
    check_near(big->evaluate(), 1e15, "Constant(1e15) evaluate");

    // Negative constant
    auto negc = std::make_shared<Constant>(-0.5);
    check_near(negc->evaluate(), -0.5, "Constant(-0.5) evaluate");
    auto t2  = make_table();
    auto vy  = std::make_shared<Variable>("y", t2); // y=3
    auto mul = std::make_shared<Mul>(negc, vy);
    check_near(mul->evaluate(), -1.5, "Mul(-0.5, y) evaluate == -1.5");
}

// ===========================================================================
// main
// ===========================================================================
int main()
{
    std::cout << "Expression Tree — Test Suite\n";
    std::cout << "============================\n";

    test_constant();
    test_variable();
    test_unary_minus();
    test_add();
    test_sub();
    test_mul();
    test_div();
    test_composite();
    test_deep();
    test_shared_subtrees();
    test_operator_stream();
    test_edge_cases();

    std::cout << "\n============================\n";
    std::cout << "Results: " << g_passed << " / " << g_total << " passed\n";

    return (g_passed == g_total) ? 0 : 1;
}
