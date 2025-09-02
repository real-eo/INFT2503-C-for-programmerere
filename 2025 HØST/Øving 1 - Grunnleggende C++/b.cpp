#include <iostream>
#include <fstream>


// Funksjonsprototype
void read_temperatures(double temperatures[], int length);


int main() {
    const int LENGTH = 5;                                           // Antall temperaturer vi ønsker å lese
    double temperatures[LENGTH];

    // Kall på funksjonen for å lese temperaturene fra fil
    read_temperatures(temperatures, LENGTH);

    // Teller antall i intervallene
    int tempCountUnder10 = 0, tempCountBetween10and20 = 0, tempCountOver20 = 0;

    for (double temperature : temperatures) {
        if (temperature < 10)       { tempCountUnder10++; }
        else if (temperature > 20)  { tempCountOver20++; }
        else                        { tempCountBetween10and20++; }
    }

    // Skriver resultatet
    std::cout << "Antall under 10 er " << tempCountUnder10 << std::endl;
    std::cout << "Antall mellom 10 og 20 er " << tempCountBetween10and20 << std::endl;
    std::cout << "Antall over 20 er " << tempCountOver20 << std::endl;

    return 0;
}
    
// Funksjonsdefinisjon - plassert etter main                            // Vi kunne også brukt `double* tempratures`, men gjør 
void read_temperatures(double temperatures[], int length) {             // slik for å gjøre det mer tydlig hva `tempratures` er
    std::ifstream file("tempratures.txt");                          // Åpner filen.
    
    if (!file) {                                                    // Sørg for at filen åpnes riktig
        throw std::runtime_error(
            "Filen kunne ikke åpnes! "
            "Har du husket å legge filen i samme "                  // Merk: Filen må ligge i samme mappe som det kompilerte programmet
            "mappe som det kompilerte programmet?"
        ); 

    }

    for (int i = 0; i < length; i++) {
        file >> temperatures[i];                                    // Lese inn tallene til et array
    }

    file.close();
}
