#include <iostream>

int main() {
    double number;
    double *p = &number;    // en peker til `number`

    // Tre ulike måter å tilordne verdi til number (dersom man ikke skal lage noen nye variabler):
    number = 3.14;          // 1. direkte tilordning
    std::cout << "number: " << number << std::endl; 
    
    *p = 2.71;              // 2. tilordning via pekeren
    std::cout << "number: " << number << std::endl; 

    p[0] = 1.618;           // 3. en annen variant av tilordning via pekeren
    std::cout << "number: " << number << std::endl; 

    // Eventuelt, dersom man tillater opprettelsen av nye variabler:
    double &ref = number;
    ref = 0.577;            // 4. tilordning via referanse
    std::cout << "number: " << number << std::endl; 


    return 0;
}