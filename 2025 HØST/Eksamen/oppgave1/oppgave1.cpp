#include <iostream>
#include <vector>
#include <string>
#include <type_traits>


// Artimetiske typer
template <typename T>
constexpr typename std::enable_if<std::is_arithmetic<T>::value, T>::type
times_two(T x) {
    return x * 2;
}


// Streng
std::string times_two(std::string s) {      // PBV, ettersom at typen ikke er
    return s + s;                           // veldig stor i dette tilfellet
}

// Vector
std::vector<int> times_two(const std::vector<int>& v) {
    std::vector<int> r;
    
    r.reserve(v.size());
    
    for (int x : v) {
        r.emplace_back(x * 2);
    }
    
    return r;
    
}

// Utskrift av vector; - operator<< overload
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "{ ";
    
    for (std::size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        
        if (i + 1 < v.size()) {
            os << ", ";
        }
    }
    
    os << " }";
    
    return os;
}


// Usikker på om jeg bare skulle implementere funksjonene, eller om jeg 
// skulle inkludere main. Inkluderer derfor main for sikkerhetens skyld.
int main() {
    std::cout << times_two(2) << std::endl;         // Jeg hadde nok heller valgt "\n"
    std::cout << times_two(2.25) << std::endl;      // for å unngå å flushe, men
    std::cout << times_two("test") << std::endl;    // men macher oppgaven for sikkerhetens skyld
    std::cout << times_two(std::vector<int>{1, 2, 3}) << std::endl;
    
    return 0;
}





