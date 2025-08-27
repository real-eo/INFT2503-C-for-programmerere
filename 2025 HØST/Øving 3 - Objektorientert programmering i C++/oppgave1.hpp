#pragma once
const double pi = 3.141592;

class Circle {
    public:
        Circle(double radius_);
        int get_area() const;
        double get_circumference() const;
    private:
        double radius;
};

// ==> Implementasjon av klassen Circle

Circle::Circle(double radius_) : radius(radius_) {}

int Circle::get_area() const {                      // Ikke en feil, men en bug. Avrundingsfeil i return verdien pga narrowing fra double -> int 
    return pi * radius * radius;                    // Usikker på om dette er med vilje eller ikke, men lar stå, selv om jeg personlig hadde gjort
}                                                   // return type om til en double  

double Circle::get_circumference() const {
    return 2.0 * pi * radius;
}
