#include <iostream>
#include <type_traits>
#include <cmath>


template <typename T>
class Angle {
    static_assert(std::is_arithmetic<T>::value, "Angle krever aritmetisk type");

private:
    T value_;

    // Normaliseringsfunksjon: grader hvis integral-type, altså tall uten desimaler, ellers anta radianer hvis flyttall
    static T normalize(T v) {
        if (std::is_integral<T>::value) {
            // Normaliser til [0, 360)
            long long vv = static_cast<long long>(v);
            long long m = vv % 360;

            if (m < 0) {
                m += 360;
            };
        
            return static_cast<T>(m);
        
        } else {
            // Normaliser til [0, 2*pi)
            double dv = static_cast<double>(v);
            double mod = std::fmod(dv, 2.0 * M_PI);

            if (mod < 0) {
                mod += 2.0 * M_PI;
            };
            
            return static_cast<T>(mod);
        }
    }

public:
    // Konstruer fra T
    explicit Angle(T v) : value_(normalize(v)) {}

    // Konstruer fra annen aritmetisk type; - konverteres til T
    template <typename U, typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
    Angle(U v) : value_(normalize(static_cast<T>(v))) {}

    T value() const { return value_; }

    // Legg til en aritmetisk verdi; resultatet er fortsatt Angle<T>
    template <typename U, typename = typename std::enable_if<std::is_arithmetic<U>::value>::type>
    Angle operator+(U rightHandSide) const {
        T sum = value_ + static_cast<T>(rightHandSide);

        return Angle(sum);              // konstruktøren normaliserer igjen
    }

};

// Utskriftsoperator: viser den normaliserte verdien med standard stream-format
template <typename T>
std::ostream& operator<<(std::ostream& os, Angle<T> const& a) {
    os << a.value();

    return os;
}

int main() {
    std::cout << Angle<int>(30) << std::endl;
    std::cout << (Angle<int>(30) + 360) << std::endl;
    std::cout << (Angle<int>(30) + 3.14159265359) << std::endl;
    std::cout << Angle<double>(3.14159265359 / 2) << std::endl;
    std::cout << (Angle<double>(3.14159265359 / 2) + 3.14159265359) << std::endl;
    std::cout << (Angle<double>(3.14159265359 / 2) + 3) << std::endl;

    return 0;
}