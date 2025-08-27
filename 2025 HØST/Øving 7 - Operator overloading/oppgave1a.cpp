#include "src/fraction.hpp"
#include <iostream>

// For deklarasjon, se: "src/fraction.hpp"
Fraction Fraction::operator-(const int& value) {
    return Fraction(numerator - denominator * value, denominator);
}

// For deklarasjon, se: "src/fraction.hpp"
Fraction operator-(const int& value, const Fraction& fraction){
    return Fraction(value * fraction.denominator - fraction.numerator, fraction.denominator);
}


int main() {
    Fraction fraction1(1, 2);

    std::cout << (fraction1 - 5) << std::endl;
    std::cout << (5 - fraction1) << std::endl;

    return 0;
}
