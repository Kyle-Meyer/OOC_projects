#pragma once
#include <iostream>

class Probability {
public:
    Probability();
    Probability(double val);
    Probability& operator=(double val);

    double value() const;

    Probability& operator&=(const Probability& rhs);
    Probability& operator|=(const Probability& rhs);
    Probability& operator^=(const Probability& rhs);
    Probability& operator-=(const Probability& rhs);
    Probability  operator~() const;

    friend std::ostream& operator<<(std::ostream& os, const Probability& p);

private:
    double p_;
    static double clamp(double v);
};

Probability operator&(Probability lhs, const Probability& rhs);
Probability operator|(Probability lhs, const Probability& rhs);
Probability operator^(Probability lhs, const Probability& rhs);
Probability operator-(Probability lhs, const Probability& rhs);
