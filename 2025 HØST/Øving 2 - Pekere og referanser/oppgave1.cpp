#include <iostream>

int main() {
    // * Oppgave a)
    int i = 3;
    int j = 5;
    int *p = &i;
    int *q = &j;

    // Write out the different values and addresses for each variable
    std::cout << "Value of i: " << i << ", Address of i: " << &i << std::endl;
    std::cout << "Value of j: " << j << ", Address of j: " << &j << std::endl;
    std::cout << "Value of p: " << p << " (" << *p << "), Address of p: " << &p << std::endl;
    std::cout << "Value of q: " << q << " (" << *q << "), Address of q: " << &q << std::endl;

    // * Oppgave b)
    *p = 7;
    *q += 4;
    *q = *p + 1;
    p = q;
    std::cout << *p << " " << *q << std::endl;

    // Write out the different values and addresses after modifications
    std::cout << "After modifications:" << std::endl;
    std::cout << "Value of i: " << i << ", Address of i: " << &i << std::endl;
    std::cout << "Value of j: " << j << ", Address of j: " << &j << std::endl;
    std::cout << "Value of p: " << p << " (" << *p << "), Address of p: " << &p << std::endl;
    std::cout << "Value of q: " << q << " (" << *q << "), Address of q: " << &q << std::endl;

    return 0;
}