#include <vector>
#include <iostream>
#include <algorithm>

int main() {
    std::vector<double> numbers = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    std::cout << "std::vector<T,Allocator>::front(): " << numbers.front() << std::endl;
    std::cout << "std::vector<T,Allocator>::back(): "  << numbers.back()  << std::endl;

    numbers.emplace(numbers.begin() + 1, 1.5);

    std::cout << "std::vector<T,Allocator>::front() etter emplace: " << numbers.front() << std::endl;

    auto it = std::find(numbers.begin(), numbers.end(), 1.5);
    
    if (it != numbers.end()) {
        std::cout << "Found: " << *it << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}