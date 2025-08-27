#include <iostream>

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair(T1 a, T2 b) : first(a), second(b) {}

    // Elementvis summering
    Pair operator+(const Pair& other) const {
        return Pair(first + other.first, second + other.second);
    }

    // Sammenligning basert på summen av elementene
    bool operator>(const Pair& other) const {
        return (first + second) > (other.first + other.second);
    }
};

int main() {
    Pair<double, int> p1(3.5, 14);
    Pair<double, int> p2(2.1, 7);
    std::cout << "p1: " << p1.first << ", " << p1.second << std::endl;
    std::cout << "p2: " << p2.first << ", " << p2.second << std::endl;

    if (p1 > p2)
        std::cout << "p1 er størst" << std::endl;
    else
        std::cout << "p2 er størst" << std::endl;

    auto sum = p1 + p2;
    std::cout << "Sum: " << sum.first << ", " << sum.second << std::endl;
}