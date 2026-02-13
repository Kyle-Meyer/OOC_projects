#include "Probability.hh"
#include <iostream>

int main() {
    Probability A(0.5);
    Probability B(0.3);

    std::cout << "A = " << A << "\n";
    std::cout << "B = " << B << "\n\n";

    // Binary operators
    std::cout << "A & B  = " << (A & B)  << "  (both: expected 0.15)\n";
    std::cout << "A | B  = " << (A | B)  << "  (either: expected 0.65)\n";
    std::cout << "A ^ B  = " << (A ^ B)  << "  (exclusive or: expected 0.5)\n";
    std::cout << "A - B  = " << (A - B)  << "  (A but not B: expected 0.35)\n";
    std::cout << "~A     = " << ~A       << "  (not A: expected 0.5)\n";
    std::cout << "~B     = " << ~B       << "  (not B: expected 0.7)\n\n";

    // Compound assignment
    Probability C(0.4);
    std::cout << "C = " << C << "\n";
    C &= A;
    std::cout << "C &= A -> " << C << "  (expected 0.2)\n";

    C = 0.4;
    C |= A;
    std::cout << "C |= A -> " << C << "  (expected 0.7)\n";

    C = 0.4;
    C ^= A;
    std::cout << "C ^= A -> " << C << "  (expected 0.5)\n";

    C = 0.4;
    C -= A;
    std::cout << "C -= A -> " << C << "  (expected 0.2)\n\n";

    // Compound expression
    std::cout << "(A | B) & ~C  with C=0.4:\n";
    C = 0.4;
    std::cout << "  = " << ((A | B) & ~C) << "  (expected 0.39)\n\n";

    // Implicit conversion from double
    Probability D = A & 0.3;
    std::cout << "A & 0.3 = " << D << "  (expected 0.15)\n";
    D = 0.6 | A;
    std::cout << "0.6 | A = " << D << "  (expected 0.8)\n\n";

    // Clamping invalid values
    Probability bad1(-0.5);
    Probability bad2(1.5);
    std::cout << "Probability(-0.5) = " << bad1 << "  (clamped to 0)\n";
    std::cout << "Probability(1.5)  = " << bad2 << "  (clamped to 1)\n";

    bad1 = 2.0;
    std::cout << "assign 2.0        = " << bad1 << "  (clamped to 1)\n";
    bad1 = -1.0;
    std::cout << "assign -1.0       = " << bad1 << "  (clamped to 0)\n\n";

    // Edge cases
    Probability zero(0.0);
    Probability one(1.0);
    std::cout << "0.0 & 1.0 = " << (zero & one) << "  (expected 0)\n";
    std::cout << "0.0 | 1.0 = " << (zero | one) << "  (expected 1)\n";
    std::cout << "~0.0      = " << ~zero        << "  (expected 1)\n";
    std::cout << "~1.0      = " << ~one         << "  (expected 0)\n";

    return 0;
}
