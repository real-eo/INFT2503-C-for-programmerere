#pragma once
#include <string>

class Commodity {
private:
    std::string name;
    int id;
    double price;
public:
    Commodity(const std::string& name, int id, double price);

    std::string get_name() const;
    int get_id() const;
    double get_price(double quantity = 1) const;
    void set_price(double newPrice);
    double get_price_with_sales_tax(double quantity = 1) const;
};
