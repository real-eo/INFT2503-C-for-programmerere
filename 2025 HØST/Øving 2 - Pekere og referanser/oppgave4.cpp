int main() {
    int a = 5;  
    int &b = a;         // FEIL: `int &b;` krever initialisering ettersom at `&b` er en referanse, og dermed må ha en lvalue å referere til.
    int *c;
    c = &b;
    a = b + *c;         // FEIL: `*a = *b + *c;` - både `*a` og `*b` er ugyldig fordi verken er pekere.
    b = 2;              // FEIL: `&b = 2;` er ugyldig, `&b` er en adresse (lvalue), ikke en variabel.

    return 0;
}