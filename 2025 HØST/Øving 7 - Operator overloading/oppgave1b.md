### Forklaring av `5 - 3 - fraction1 - 7 - fraction2`

Først og fremst, så har `-` venstreassosiativitet, det vil si at uttrykket evalueres **fra venstre til høyre**:  
```cpp
((((5 - 3) - fraction1) - 7) - fraction2)
```


#### Stegvis operatorvalg

1. `5 - 3`: Vanlig standard C++ operator, `int - int`.
2. `2 - fraction1`: **ikke-medlemsfunksjon**, `int - Fraction`
    ```cpp
    Fraction operator-(int, const Fraction&)
    ```
3. `<Fraction> - 7`: **medlemsfunksjon**, `Fraction - int`

   ```cpp
   Fraction Fraction::operator-(int) const
   ```
4. `<Fraction> - fraction2`: **medlemsfunksjon**, `Fraction - Fraction`
   
   ```cpp
   Fraction Fraction::operator-(const Fraction&) const
   ```