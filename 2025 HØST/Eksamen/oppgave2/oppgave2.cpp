#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <utility>


// Abstrakt base klasse
class BuildSystem {                                     // Strengt tatt, kunne denne, og de påfølgende klassene vært 
public:                                                 // structs, ettersom alt er public som standard
    virtual ~BuildSystem() {}
    virtual std::string buildCommand() const = 0;
};


class CMake : public BuildSystem {
public:
    std::string buildCommand() const override { return "cmake --build ."; }
};

class Meson : public BuildSystem {
public:
    std::string buildCommand() const override { return "meson compile -C ."; }
};

class Cargo : public BuildSystem {
public:
    std::string buildCommand() const override { return "cargo build"; }
};



// Abstrakt base klasse for programmeringsspråk
class ProgrammingLanguage {
protected:
    std::unique_ptr<BuildSystem> buildSystem_;

public:
    ProgrammingLanguage() {}
    explicit ProgrammingLanguage(std::unique_ptr<BuildSystem> bs) : buildSystem_(std::move(bs)) {}
    virtual ~ProgrammingLanguage() {}

    // Disse holder ProgrammingLanguage abstrakt slik at forsøk på å instansiere klassen gir kompilasjonsfeil
    virtual std::string print_line(const std::string& s) const = 0;
    virtual std::string for_each(const std::string& elementName, const std::string& containerName) const = 0;

    // Default implementasjon som delegerer til et valgfritt BuildSystem
    virtual std::string build() const {
        if (buildSystem_) { 
            return buildSystem_->buildCommand();
        }

        return std::string();
    }
};


// C++ implementasjon
class Cpp : public ProgrammingLanguage {
public:
    Cpp() : ProgrammingLanguage() {}
    explicit Cpp(std::unique_ptr<BuildSystem> bs) : ProgrammingLanguage(std::move(bs)) {}

    std::string print_line(const std::string& s) const override {
        return "cout << \"" + s + "\" << endl;";
    }

    std::string for_each(const std::string& elementName, const std::string& containerName) const override {
        return "for(auto &" + elementName + ": " + containerName + ") {}";
    }
};

// Rust implementasjon
class Rust : public ProgrammingLanguage {
public:
    Rust() : ProgrammingLanguage() {}
    explicit Rust(std::unique_ptr<BuildSystem> bs) : ProgrammingLanguage(std::move(bs)) {}

    std::string print_line(const std::string& s) const override {
        return "println!(\"" + s + "\");";
    }

    std::string for_each(const std::string& elementName, const std::string& containerName) const override {
        return "for " + elementName + " in &" + containerName + " {}";
    }
};


int main() {
    // NB:  Jeg har valgt å implementere begge delene i hver sin scope
    //       for å unngå at variablene fra a) skal forstyrre i b)

    // a)
    {    
        std::vector<std::unique_ptr<ProgrammingLanguage>> programming_languages;
        // Uncommenting the following line should cause compilation error:
        // programming_languages.emplace_back(std::make_unique<ProgrammingLanguage>());

        programming_languages.emplace_back(std::make_unique<Cpp>());
        programming_languages.emplace_back(std::make_unique<Rust>());

        for (auto &programming_language : programming_languages) {
            std::cout << programming_language->print_line("Hello World") << std::endl;      // Samme greia her med flushing, men men
            std::cout << programming_language->for_each("e", "vec") << std::endl
                      << std::endl;
        }
    }

    // b)
    {
        std::vector<std::unique_ptr<ProgrammingLanguage>> programming_languages;
        programming_languages.emplace_back(std::make_unique<Cpp>(std::make_unique<CMake>()));
        programming_languages.emplace_back(std::make_unique<Cpp>(std::make_unique<Meson>()));
        programming_languages.emplace_back(std::make_unique<Rust>(std::make_unique<Cargo>()));

        for (auto &programming_language : programming_languages) {
            std::cout << programming_language->build() << std::endl;
        }
    }

    return 0;
}