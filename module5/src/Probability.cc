#include "Probability.hh"
#include <algorithm>

double Probability::clamp(double v) {
    return std::max(0.0, std::min(1.0, v));
}

Probability::Probability() : p_(0.0) {}

Probability::Probability(double val) : p_(clamp(val)) {}

Probability& Probability::operator=(double val) {
    p_ = clamp(val);
    return *this;
}

double Probability::value() const {
    return p_;
}

Probability& Probability::operator&=(const Probability& rhs) {
    p_ = clamp(p_ * rhs.p_);
    return *this;
}

Probability& Probability::operator|=(const Probability& rhs) {
    p_ = clamp(p_ + rhs.p_ - p_ * rhs.p_);
    return *this;
}

Probability& Probability::operator^=(const Probability& rhs) {
    p_ = clamp(p_ + rhs.p_ - 2.0 * p_ * rhs.p_);
    return *this;
}

Probability& Probability::operator-=(const Probability& rhs) {
    p_ = clamp(p_ * (1.0 - rhs.p_));
    return *this;
}

Probability Probability::operator~() const {
    return Probability(1.0 - p_);
}

std::ostream& operator<<(std::ostream& os, const Probability& p) {
    return os << p.p_;
}

Probability operator&(Probability lhs, const Probability& rhs) {
    lhs &= rhs;
    return lhs;
}

Probability operator|(Probability lhs, const Probability& rhs) {
    lhs |= rhs;
    return lhs;
}

Probability operator^(Probability lhs, const Probability& rhs) {
    lhs ^= rhs;
    return lhs;
}

Probability operator-(Probability lhs, const Probability& rhs) {
    lhs -= rhs;
    return lhs;
}
