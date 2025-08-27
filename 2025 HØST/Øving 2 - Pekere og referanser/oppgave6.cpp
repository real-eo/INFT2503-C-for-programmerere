#include <iostream>

int find_sum(const int *table, int length) {
    int sum = 0;

    for (int i = 0; i < length; i++) {
        sum += table[i];
    }

    return sum;
}


int main() {
    int table[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    int sum10First = find_sum(table, 10);
    int sum5After = find_sum(table + 10, 5);          
    int sum5Last = find_sum(table +                 // Start of the array
        (sizeof(table) / sizeof(table[0]))          // Calculate the number of elements in the array
        - (5)                                       // Subtract the amount of elements we want to calculate
    , 5);

    std::cout << "Summen av de 10 foerste elementene i tabellen er: " << sum10First << std::endl;
    std::cout << "Summen av de 5 elementene etter den 10. er: " << sum5After << std::endl;
    std::cout << "Summen av de 5 siste elementene er: " << sum5Last << std::endl;

    std::cout << "Total: " << sum10First + sum5After + sum5Last << std::endl;

    return 0;
}