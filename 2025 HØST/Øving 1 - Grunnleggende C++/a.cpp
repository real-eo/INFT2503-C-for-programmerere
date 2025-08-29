#include <iostream>
#include <ostream>


int main() {
    std::cout << "Du skal skrive inn 5 temperaturer." << std::endl;

    // Les inn temperaturer fra brukeren
    double temperaturer[5];
    for (int i = 0; i < 5; ++i) {
        std::cout << "Temperatur nr " << (i + 1) << ": ";
        std::cin >> temperaturer[i];
    }

    // Grupper
    int antallUnder10 = 0, antallMellom10og20 = 0, antallOver20 = 0;
    for (double temperatur : temperaturer) {
        if (temperatur < 10)        { antallUnder10++; }
        else if (temperatur > 20)   { antallOver20++; }
        else                        { antallMellom10og20++; }
    }

    std::cout << "Antall under 10 er " << antallUnder10 << std::endl;
    std::cout << "Antall mellom 10 og 20 er " << antallMellom10og20 << std::endl;
    std::cout << "Antall over 20 er " << antallOver20 << std::endl;

    return 0;
}