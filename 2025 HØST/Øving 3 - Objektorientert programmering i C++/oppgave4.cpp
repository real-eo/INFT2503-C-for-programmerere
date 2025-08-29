#include <string>
#include <ostream>
#include <iostream>
#include <algorithm>


void inline task(const char taskIdentifier) {                                       // Funksjon for å gjøre output mer ryddig
    std::cout << "\n" << taskIdentifier << ")" << std::endl;
}

std::string create_sentence(std::string word1, const std::string& word2, const std::string& word3) {
    word1[0] = toupper(word1[0]);                                                   
    return word1 + " " + word2 + " " + word3 + ".";
}

// Her velger jeg å bruke 4 parametre fremfor et array, ettersom at 4 parametre er enklere og tydligere i et scenario hvor jeg vet
// antall ord på forhånd. Ellers hadde jeg valgt enten et array eller en vector ettersom at en slik løsning er mer skalerbar
void display_lengths(const std::string& word1, const std::string& word2, const std::string& word3, const std::string& sentence) {
    std::cout << "Length of word 1: " << word1.length() << std::endl;
    std::cout << "Length of word 2: " << word2.length() << std::endl;
    std::cout << "Length of word 3: " << word3.length() << std::endl;
    std::cout << "Length of sentence: " << sentence.length() << std::endl;
}

void manipulate_sentence(const std::string& sentence) {
    std::string sentence2 = sentence;
    const int UPPER_BOUND = std::min(static_cast<int>(sentence2.length()), 12);     // En øvre grense slik at for-løkken ikke 
                                                                                    // erstatter noe lengre enn til indeks 11
    
    for (int i = 9; i < UPPER_BOUND; i++) {     // Merk: Her er det fullt mulig å bruke `std::string::replace(pos, len, str)`
        sentence2[i] = 'x';                     //       istedenfor en for-løkke, men velger å ikke, ettersom at denne løsningen    
    }                                           //       ikke behøver en sjekk for nedre grense på lengden til setningen - er  
                                                //       den for kort, så kjøres aldri koden i første omgang. 
                                                
    std::cout << "Original sentence: " << sentence << std::endl;
    std::cout << "Modified sentence: " << sentence2 << std::endl;
}

void start_of_sentence(const std::string& sentence) {
    const int UPPER_BOUND = std::min(static_cast<int>(sentence.length()), 5);       // Samme grunn som i manipulate_sentence()
    std::string sentence_start = sentence.substr(0, UPPER_BOUND);

    std::cout << "Original sentence: " << sentence << std::endl;
    std::cout << "Start of sentence: " << sentence_start << std::endl;
}

bool contains_word(std::string sentence, std::string word) {
    // Merk: Vi gjør begge strenger til små bokstaver ettersom at vi kun er ute 
    //       etter om setningen inneholder et spesifikt ord, ikke substring

    std::transform(sentence.begin(), sentence.end(), sentence.begin(), ::tolower);
    // std::transform(word.begin(), word.end(), word.begin(), ::tolower);   // Blir unødvendig i akkuratt dette tilfellet  
                                                                            // fordi vi vet at "hallo" ikke inneholder noen
                                                                            // store bokstaver. Men ellers må dette gjøres.
    const bool CONTAINS = (sentence.find(word) != std::string::npos);

    std::cout << "The sentence " 
              << (CONTAINS ? "contains" : "does not contain") 
              << " the word \"" << word << "\"." << std::endl;
    
    return CONTAINS;
}

void all_occurrences(const std::string& sentence, const std::string& substring) {
    // Merk: Ettersom at oppgaven spesifikt spør om strengen "er", må det skilles 
    //       på forekomster av "eR", "Er", "ER", og "er" i setningen.

    // std::transform(sentence.begin(), sentence.end(), sentence.begin(), ::tolower);   // Dermed, hvorfor vi ikke bør gjøre dette
    
    size_t pos = 0;
    size_t count = 0;

    while ((pos = sentence.find(substring, pos)) != std::string::npos) {
        std::cout << "Found occurrence of \"" << substring << "\" at index: " << pos << std::endl;
        pos += substring.length();
        count++;
    }

    if (count == 0) {
        std::cout << "No occurrences found." << std::endl;
        return;
    }
    
    std::cout << "A total of " << count << " occurrence" << (count != 1 ? "s" : "") << " was found." << std::endl;
}

int main() {
    std::string word1;
    std::string word2;
    std::string word3;

    // a)
    task('a');
    std::cout << "Skriv inn tre ord (separer ordene med kun mellomrom): ";
    std::cin >> word1 >> word2 >> word3;

    // b)
    task('b');
    std::string sentence = create_sentence(word1, word2, word3);
    std::cout << "Sentence: " << sentence << std::endl;

    // c)
    task('c');
    display_lengths(word1, word2, word3, sentence);

    // d) og e)                         Merk: Jeg lager ikke en kopi av `sentence` i main funksjonen fordi `sentence2` 
    task('d'); task('e');               //    ikke brukes noe vidre etter oppgave e), og dermed gjør det enklere ved 
    manipulate_sentence(sentence);      //    å la sentence2 kun være i live under levetiden til manipulate_sentence

    // f)
    task('f');
    start_of_sentence(sentence);

    // g)
    task('g');
    contains_word(sentence, "hallo");

    // h)
    task('h');
    all_occurrences(sentence, "er");


    return 0;
}