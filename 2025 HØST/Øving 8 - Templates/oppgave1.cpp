#include <iostream>
#include <iomanip>
#include <cmath>

// Funksjonstemplate
template <typename Type>
bool equal(Type a, Type b) {
    std::cout << "Template-versjonen brukes\n";
    return a == b;
}

// Spesialisering for double
template <>
bool equal<double>(double a, double b) {
    std::cout << "Spesialisert double-versjon brukes\n";
    return std::fabs(a - b) < 0.00001;
}

int main() {
    int x = 5, y = 5;
    double d1 = 0.123456789, d2 = 0.123456780;

    std::cout << std::boolalpha;

    // Test med int
    std::cout << "Sammenligner int: " << x << " og " << y << "\n";
    std::cout << "Resultat: " << equal(x, y) << "\n\n";

    // Test med double
    std::cout << std::setprecision(10);
    std::cout << "Sammenligner double: " << d1 << " og " << d2 << "\n";
    std::cout << "Resultat: " << equal(d1, d2) << "\n";

    return 0;
}