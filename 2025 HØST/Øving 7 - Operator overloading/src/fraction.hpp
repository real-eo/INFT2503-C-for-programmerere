//
// src/fraction.hpp
//
#pragma once

#include <iostream>

class Fraction {
public:
    int numerator;
    int denominator;

    Fraction();
    Fraction(int numerator, int denominator);
    Fraction(const Fraction& other);  
    void set(int numerator_, int denominator_ = 1);
    Fraction operator+(const Fraction &other) const;
    Fraction operator-(const Fraction &other) const;
    Fraction operator*(const Fraction &other) const;
    Fraction operator/(const Fraction &other) const;
    Fraction operator-() const;
    Fraction &operator++(); // Preinkrement
    Fraction &operator--(); // Predekrement
    Fraction &operator+=(const Fraction &other);
    Fraction &operator-=(const Fraction &other);
    Fraction &operator*=(const Fraction &other);
    Fraction &operator/=(const Fraction &other);
    Fraction &operator=(const Fraction &other);
    bool operator==(const Fraction &other) const;
    bool operator!=(const Fraction &other) const;
    bool operator<=(const Fraction &other) const;
    bool operator>=(const Fraction &other) const;
    bool operator<(const Fraction &other) const;
    bool operator>(const Fraction &other) const;
    friend std::ostream &operator<<(std::ostream &os, const Fraction &fraction);

    // For oppgave a)
    Fraction operator-(const int& value);
    friend Fraction operator-(const int& value, const Fraction& fraction);

private:
    void reduce();
    int compare(const Fraction &other) const;
};