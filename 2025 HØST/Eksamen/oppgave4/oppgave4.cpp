#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <type_traits>


// Fabrikk-wrapper-type som holder predikatet
template <typename Pred>
class ParallellFilterImpl {                                         // Denne klassen kunne like gjerne vært en struct
public:
    Pred pred;
    explicit ParallellFilterImpl(Pred p) : pred(std::move(p)) {}
};


// Deduction hjelper-funksjon slik at kalleren kan skrive ParallellFilter(lambda)
template <typename Pred>
ParallellFilterImpl<typename std::decay<Pred>::type> ParallellFilter(Pred&& p) {
    return ParallellFilterImpl<typename std::decay<Pred>::type>(std::forward<Pred>(p));
}


// operator|: anvender ParallellFilter på en container. Merk; forventer random-access container som f.eks std::vector
template <typename Container, typename Pred>
auto operator|(const Container& c, const ParallellFilterImpl<Pred>& pf)
    -> std::vector<std::vector<typename Container::value_type>>     // Her har jeg valgt å bruke -> syntax for å gjøre det 
{                                                                   // enklere å lese ettersom at return-typen er nokså lang
    unsigned int hw = std::thread::hardware_concurrency();
    std::size_t nthreads = (hw == 0 ? 1u : static_cast<std::size_t>(hw));       // 1u, ettersom at size_t er unsigned

    std::vector<std::vector<typename Container::value_type>> results(nthreads);

    // Start opp tråder: hver tråd prosesserer indekser i = tid, tid + nthreads, tid + 2*nthreads, ...
    std::vector<std::thread> threads;
    threads.reserve(nthreads);

    for (std::size_t tid = 0; tid < nthreads; ++tid) {
        threads.emplace_back([&, tid]() {
            for (std::size_t i = tid; i < c.size(); i += nthreads) {
                if (pf.pred(c[i])) {
                    results[tid].emplace_back(c[i]); // hvor hver tråd skriver til sin egen vektor -> ingen synkronisering nødvendig
                }
            }
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    return results;
}


// Utskriftsoperator: viser innholdet i en std::vector<T> på form: { elem1, elem2, ... }
// Fungerer rekursivt for nestede vektorer.
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "{ ";
    
    for (std::size_t i = 0; i < v.size(); ++i ) {
        os << v[i];
        
        if (i + 1 < v.size()) {
            os << ", ";
        }
    
    }
    
    os << " }";
    
    return os;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Note: nummeret av indre vektorer er lik std::thread::hardware_concurrency() (fallback 1 hvis 0).
    std::cout << (vec | ParallellFilter([](int e) { return e % 2 == 0; })) << std::endl;
    std::cout << (vec | ParallellFilter([](int e) { return e % 2 == 1; })) << std::endl;

    return 0;
}
