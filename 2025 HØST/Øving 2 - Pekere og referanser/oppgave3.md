# Oppgave 3

Programbiten:

```cpp
char text[5];
char *pointer = text;
char search_for = 'e';
cin >> text;
while (*pointer != search_for) {
  *pointer = search_for;
  pointer++;
}
```

Feil/risikoer:

- `cin >> text` kan overskride bufferet (text har plass for 4 tegn + `'\0'`). Dette gir buffer overflow.
- Løkken sjekker bare `*pointer != search_for`. Hvis `search_for` ikke finnes i strengen, vil pointer gå forbi slutt-nul (`'\0'`) og fortsette å lese/ skrive utenfor buffer — undefined behavior / mulig infinite loop hvis aldri treffer `search_for`.
- Kroppen `*pointer = search_for;` overskriver data i strengen uten kontroll — sannsynlig bug (gresk logikk).
- Ingen sjekk for enden av streng (`'\0'`) eller for maksimal buffer-størrelse.
