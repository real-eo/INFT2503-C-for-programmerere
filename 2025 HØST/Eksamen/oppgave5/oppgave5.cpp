// KI-hjelp: Rammestruktur generert av GitHub Coplot (ChatGPT GPT-5 mini) 2025-11-24. Shunting-yard / RPN implementasjon rettet opp i av GitHub Coplot (ChatGPT GPT-5 mini) 2025-11-24. Alt output er manuelt revidert.
// TODO: FIKSE DESIMALTALL MULTIPLIKASJONS AVRUNDINGSFEIL
// TODO: LEGGE TIL PARENTESER I UI

#include <gtkmm.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <stdexcept>


class CalculatorWindow : public Gtk::Window {
private:
    // UI
    Gtk::Box vbox_{Gtk::ORIENTATION_VERTICAL};
    Gtk::Entry display_;
    Gtk::Grid grid_;

    // State
    char selectedOperator = '%';                                                         // currently selected operator
    bool unaryMinusUsed = true;

    // * Helpers
    void addButton(const std::string& label, int col, int row, int width = 1, int height = 1) {
        auto btn = Gtk::manage(new Gtk::Button(label));

        btn->signal_clicked().connect(sigc::bind<std::string>(sigc::mem_fun(*this, &CalculatorWindow::onButtonClicked), label));
        
        grid_.attach(*btn, col, row, width, height);
    }

    void addText(const std::string& text) {
        std::string current = display_.get_text();
        display_.set_text(current + text);
    }

    void removeText(int count) {
        std::string current = display_.get_text();

        // Safety check, in case of invalid count
        if (current.size() < static_cast<size_t>(count)) {
            display_.set_text("");
            return;
        }

        std::string newText = current.substr(0, current.size() - count);
        display_.set_text(newText);
    }

    void changeText(const std::string& text, int backtrackIndicies) {
        removeText(backtrackIndicies);
        addText(text);
    }

    const char lastTextCharacter() const {
        std::string current = display_.get_text();

        // Safety check
        if (current.empty()) {
            std::cout << "Display is empty.\n";
            return '\0';
        }

        return current.back();
    }
    

    const bool validUnaryCase(const char operand) const {
        if (operand != '-') return false;

        return (
            display_.get_text().empty() ||
            isOperand(lastTextCharacter())
        );
    }

    // * Static functions
    static bool isOperand(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    static int prec(const std::string &op) {
        if (op == "u") return 4;
        if (op == "*" || op == "/") return 3;
        if (op == "+" || op == "-") return 2;
        return 0;
    };

    void onButtonClicked(const std::string& label) {
        switch (static_cast<char>(label[0])) {
            case 'C':
                clear();
                break;
            
            case '+': case '-': case '*': case '/': 
                std::cout << "Operator button clicked: " << label << "\n";
                
                selectedOperator = label[0];

                if (lastTextCharacter() == '-' && selectedOperator == '-') {
                    removeText(1);                                                      // remove extra unary minus
                    unaryMinusUsed = false;

                } else if (validUnaryCase(selectedOperator)) {
                    // unary minus case
                    addText(label);
                    unaryMinusUsed = true;

                } else if (display_.get_text().empty()) {
                    return;

                } else if (isOperand(lastTextCharacter())) {
                    changeText(label, 1 + unaryMinusUsed);                              // replace last operator (and unary minus if used)
                    unaryMinusUsed = false;

                } else {
                    // append operator
                    addText(label);
                }
                
                break;
            
            // ! NO BUTTON YET
            case '(':
                std::cout << "Left parenthesis button clicked\n";

                if (!display_.get_text().empty() && 
                    (std::isdigit(static_cast<unsigned char>(lastTextCharacter())) || lastTextCharacter() == ')')) {
                    // implicit multiplication
                    addText("*(");
                } else {
                    addText("(");
                }

                unaryMinusUsed = true;

                break;
            
            // ! NO BUTTON YET
            case ')':
                std::cout << "Right parenthesis button clicked\n";

                if (!display_.get_text().empty() && 
                    (std::isdigit(static_cast<unsigned char>(lastTextCharacter())) || lastTextCharacter() == ')')) {
                    addText(")");
                }

                unaryMinusUsed = false;

                break;
            
            case '=':
                std::cout << "Equals button clicked" << "\n";

                unaryMinusUsed = false;
                // Handle equals button click here
                evaluateExpression();

                break;

            case '.':
                std::cout << "Decimal point button clicked" << "\n";
                unaryMinusUsed = false;
                // Handle decimal point button click here
                break;


            default:
                // Ensure label is a digit
                if (!(label.size() == 1 && std::isdigit(static_cast<unsigned char>(label[0])))) {
                    throw std::runtime_error("Invalid number: " + label);
                    return;
                }

                std::cout << label << " digit clicked" << "\n";

                // Add operator and digit to display string
                // digit: append, replacing a single leading "0"
                addText(label);
                unaryMinusUsed = false;
            
                break;
        }

        
        std::cout << "Current display: " << display_.get_text() << "\n";
    }


    void evaluateExpression() {
        auto s = display_.get_text();
        if (s.empty()) return;

        std::vector<std::string> tokens;                                                // Tokenize (numbers, + - * / ( ) ; unary minus -> "u")

        for (size_t i = 0; i < s.size(); ) {
            char c = s[i];
            // if (std::isspace(static_cast<unsigned char>(c))) { ++i; continue; }      // Not needed as we don't allow spaces

            // Number
            if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
                size_t j = i;
                
                while (j < s.size() && (std::isdigit(static_cast<unsigned char>(s[j])) || s[j] == '.')) ++j;
                tokens.emplace_back(s.substr(i, j - i));
                
                i = j;

                continue;
            }

            // Operator or parenthesis
            if (c == '+' || c == '*' || c == '/' || c == '-' || c == '(' || c == ')') {
                if (c == '-') {
                    // unary if at start or after operator or '('
                    bool unary = tokens.empty() ||
                                 tokens.back() == "+" || tokens.back() == "-" ||
                                 tokens.back() == "*" || tokens.back() == "/" ||
                                 tokens.back() == "(";
                    if (unary) { tokens.emplace_back("u"); ++i; continue; }
                }
                tokens.emplace_back(1, c);
                ++i;
                continue;
            }

            // invalid char
            display_.set_text("Error"); return;
        }

        // Shunting-yard -> RPN
        
        auto isRightAssociative = [](const std::string &op)->bool { return op == "u"; };
        auto isUorOperand = [](const std::string &t)->bool { return t == "+" || t == "-" || t == "*" || t == "/" || t == "u"; };

        std::vector<std::string> output;
        std::vector<std::string> ops;

        for (const auto &tok : tokens) {
            if (!tok.empty() && (std::isdigit(static_cast<unsigned char>(tok[0])) || tok[0] == '.')) {
                output.emplace_back(tok);

            } else if (isUorOperand(tok)) {
                while (!ops.empty() && isUorOperand(ops.back())) {
                    std::string top = ops.back();

                    if ((!isRightAssociative(tok) && prec(tok) <= prec(top)) || (isRightAssociative(tok) && prec(tok) < prec(top))) {
                        output.emplace_back(top); ops.pop_back();

                    } else break;
                }

                ops.emplace_back(tok);

            } else if (tok == "(") {
                ops.emplace_back(tok);

            } else if (tok == ")") {
                bool found = false;

                while (!ops.empty()) {
                    if (ops.back() == "(") { 
                        ops.pop_back(); 
                        found = true; 
                        break; 
                    }

                    output.emplace_back(ops.back()); 
                    ops.pop_back();
                }

                if (!found) { 
                    display_.set_text("Error"); 
                    return; 
                }

            } else { 
                display_.set_text("Error"); 
                return; 
            }
        }
        
        while (!ops.empty()) {
            if (ops.back() == "(" || ops.back() == ")") { 
                display_.set_text("Error"); 
                return; 
            }

            output.emplace_back(ops.back()); ops.pop_back();
        }

        // Evaluate RPN
        std::vector<double> st;

        for (const auto &tok : output) {
            if (tok == "u") {
                if (st.empty()) { 
                    display_.set_text("Error");
                    return; 
                }

                double v = st.back(); 
                st.pop_back(); 
                st.emplace_back(-v);

            } else if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
                if (st.size() < 2) { 
                    display_.set_text("Error"); 
                    return; 
                }   

                double b = st.back(); st.pop_back();
                double a = st.back(); st.pop_back();
                
                     if (tok == "+") st.emplace_back(a + b);
                else if (tok == "-") st.emplace_back(a - b);
                else if (tok == "*") st.emplace_back(a * b);
                else {
                    if (b == 0.0) { display_.set_text("Error"); return; }
                
                    st.emplace_back(a / b);
                }

            } else {
                try { 
                    st.emplace_back(std::stod(tok));
                }
                
                catch (...) { 
                    display_.set_text("Error");
                    return; 
                }
            }
        }

        if (st.size() != 1) { display_.set_text("Error"); return; }

        // Format result (no trailing zeros)
        double result = st.back();
        
        if (std::isnan(result) || std::isinf(result)) { display_.set_text("Error"); return; }
        
        std::ostringstream oss;
        oss << std::setprecision(12) << std::fixed << result;
        std::string out = oss.str();

        if (out.find('.') != std::string::npos) {
            while (!out.empty() && out.back() == '0') out.pop_back();
            if (!out.empty() && out.back() == '.') out.pop_back();
        }
        
        if (out.empty()) out = "0";
        
        display_.set_text(out);
        unaryMinusUsed = false;
    }

    void clear() {
        display_.set_text("");
        unaryMinusUsed = false;
        selectedOperator = '%';
    }

public:
    CalculatorWindow() {
        set_title("GTKmm Calculator");
        set_default_size(320, 240);

        // Top-level layout
        vbox_.set_spacing(6);
        add(vbox_);

        // Display
        display_.set_editable(false);
        display_.set_hexpand(true);
        display_.set_margin_start(6);
        display_.set_margin_end(6);
        display_.set_margin_top(6);
        display_.set_margin_bottom(6);
        display_.set_text("");
        vbox_.pack_start(display_, Gtk::PackOptions::PACK_SHRINK);
        
        // Grid for buttons
        grid_.set_row_spacing(4);
        grid_.set_column_spacing(4);
        grid_.set_margin_start(6);
        grid_.set_margin_end(6);
        grid_.set_margin_top(6);
        grid_.set_margin_bottom(6);
        vbox_.pack_start(grid_);

        // Buttons layout (rows x cols)
        // Row/col layout:
        // 7 8 9 /
        // 4 5 6 *
        // 1 2 3 -
        // 0 . = +

        addButton("7", 0, 0);
        addButton("8", 1, 0);
        addButton("9", 2, 0);
        addButton("/", 3, 0);

        addButton("4", 0, 1);
        addButton("5", 1, 1);
        addButton("6", 2, 1);
        addButton("*", 3, 1);

        addButton("1", 0, 2);
        addButton("2", 1, 2);
        addButton("3", 2, 2);
        addButton("-", 3, 2);

        addButton("0", 0, 3);
        addButton(".", 1, 3);
        addButton("=", 2, 3);
        addButton("+", 3, 3);

        auto clear_btn = Gtk::manage(new Gtk::Button("C"));
        clear_btn->signal_clicked().connect(sigc::mem_fun(*this, &CalculatorWindow::clear));
        grid_.attach(*clear_btn, 0, 4, 4, 1);

        show_all_children();
    }

    virtual ~CalculatorWindow() = default;
};

int main(int argc, char** argv) {
    auto app = Gtk::Application::create(argc, argv);

    CalculatorWindow window;

    return app->run(window);
}