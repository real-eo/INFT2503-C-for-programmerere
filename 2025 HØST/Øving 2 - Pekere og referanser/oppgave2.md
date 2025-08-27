# Oppgave 2

Hva skjer med:

```cpp
char *line = nullptr; // eller char *line = 0;
strcpy(line, "Dette er en tekst");
```

Svar: Dette resulterer i undefined behavior. `strcpy` forsøker å skrive til destinasjonen (`line`) som er en nullpeker, og ikke peker til gyldig skrivbar minne fordi adressen `0x0` er reservert som "nullpekeren".
Maskinvaren/OS gir altså ikke tilgang til å bruke den som gyldig minneområde. Programmet vil mest sannsynlig krasje på grunn av segmentation fault eller korrupte minne. 
