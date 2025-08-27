#include <vector>
#include <iostream>
#include <algorithm>
#include <iostream>

class Set {
private:
    std::vector<int> elements;

public:
    Set() = default;                                                                            // Tom mengde
    Set(const Set &other) : elements(other.elements) {}

    void add(int value) {                                                                       // Legg til element (uten duplikater)
        if (std::find(elements.begin(), elements.end(), value) == elements.end())
            elements.push_back(value);
    }

    Set operator+(const Set &other) const {                                                     // Union av to mengder
        Set result = *this;
        for (int x : other.elements)
            result.add(x);                                                                      // add sjekker duplikater
        return result;
    }

    Set &operator=(const Set &other) {                                                          // Tilordning
        if (this != &other)
            elements = other.elements;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Set &s) {                          // Utskrift
        out << "{";
        for (size_t i = 0; i < s.elements.size(); ++i) {
            out << s.elements[i];
            if (i + 1 < s.elements.size())
                out << ", ";
        }
        out << "}";
        return out;
    }
};


int main() {
    Set a;
    a.add(1);
    a.add(4);
    a.add(3);

    Set b;
    b.add(4);
    b.add(7);

    std::cout << "a = " << a << "\n"; // {1, 4, 3}
    std::cout << "b = " << b << "\n"; // {4, 7}

    Set c = a + b;
    std::cout << "Union a + b = " << c << "\n"; // {1, 4, 3, 7}

    Set d;
    d = c;
    std::cout << "d = " << d << "\n"; // {1, 4, 3, 7}

    return 0;
}