#include "commodity.hpp"


Commodity::Commodity(const std::string& name, int id, double price)
    : name(name), id(id), price(price) {}

std::string Commodity::get_name() const { 
    return name; 
}

int Commodity::get_id() const { 
    return id; 
}

double Commodity::get_price(double quantity) const { 
    return price * quantity; 
}

void Commodity::set_price(double newPrice) { 
    price = newPrice; 
}

double Commodity::get_price_with_sales_tax(double quantity) const {
    const double salesTaxRate = 0.25;  // 25% sales tax
    return get_price(quantity) * (1 + salesTaxRate);
}
