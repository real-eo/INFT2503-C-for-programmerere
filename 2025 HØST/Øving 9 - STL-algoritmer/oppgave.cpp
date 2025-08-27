#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

int main() {
    // vektorer fra leksjonen
    std::vector<int> v1 = {3, 3, 12, 14, 17, 25, 30};
    std::vector<int> v2 = {2, 3, 12, 14, 24};


    // a) Finn første element i v1 som er større enn 15
    auto it = std::find_if(
        v1.begin(), v1.end(), 
        [](int x) { return x > 15; }
    );
    std::cout << "a) " << std::endl;
    if (it != v1.end()) {   std::cout << "Første element i v1 > 15: " << *it << std::endl; } 
    else {                  std::cout << "Ingen elementer i v1 > 15" << std::endl; }

    // b) Sjekk om intervallene er "omtrent like" med equal
    bool like_5 = std::equal(
        v1.begin(), v1.begin() + 5, v2.begin(),
        [](int a, int b) { return std::abs(a - b) <= 2; }
    );
    bool like_4 = std::equal(
        v1.begin(), v1.begin() + 4, v2.begin(),
        [](int a, int b) { return std::abs(a - b) <= 2; }
    );
    std::cout << "\nb) " << std::endl;
    std::cout << "Første 5 omtrent like? " << (like_5 ? "Ja" : "Nei") << std::endl;
    std::cout << "Første 4 omtrent like? " << (like_4 ? "Ja" : "Nei") << std::endl;

    // c) Erstatt alle oddetall i v1 med 100 med replace_copy_if
    std::vector<int> v3(v1.size());
    std::replace_copy_if(
        v1.begin(), v1.end(), v3.begin(),
        [](int x) { return x % 2 == 1; }, 100                                                       // Undersøker om rest er lik 1
    );
    std::cout << "\nc) " << std::endl;
    std::cout << "v1 etter erstatning: ";
    for (int x : v3) std::cout << x << " ";
    std::cout << std::endl;

    return 0;
}