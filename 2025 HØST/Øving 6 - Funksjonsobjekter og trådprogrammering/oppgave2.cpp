#include <boost/asio.hpp>                                                       // Boost.Asio - et bibliotek for nettverk/IO uten blokkering
#include <iostream>
#include <string>

using namespace std;
using namespace boost::asio::ip;                                                // Så vi kan skrive tcp istedenfor boost::asio::ip::tcp


// En enkel HTTP-server som svarer på GET-forespørsler
class SimpleHttpServer {
private:
    // "Connection"-klasse som holder en socket for én klient
    class Connection {
    public:
        tcp::socket socket;                                                     // Nettsocket for kommunikasjon med en klient
        Connection(boost::asio::io_context &io_context)                         // Konstruktør tar "IO-motoren" (se under) og
            : socket(io_context) {}                                             // initialiserer socketen knyttet til samme IO-motor
    };

    boost::asio::io_context io_context;                                         // IO-motor: kjernen som kjører alle async operasjoner og callbacks
    tcp::endpoint endpoint;                                                     // Adresse + port serveren skal lytte på
    tcp::acceptor acceptor;                                                     // Objekt som lytter etter innkommende tilkoblinger

    void handle_request(shared_ptr<Connection> connection) {
        auto read_buffer = make_shared<boost::asio::streambuf>();               // Buffer for å motta bytes fra klienten. Viktig at bufferen lever
                                                                                // lengere enn handle_request callet ettersom at håndteringen av
                                                                                // HTTP-forespørselen skjer asynkront

        async_read_until(
            connection->socket,                                                     // Hvilken socket vi leser fra
            *read_buffer,                                                           // Peker til hvor vi skal lagre dataene
            "\r\n\r\n",                                                             // Delimiter: dobbel linjeskift avslutter headers
            [this, connection, read_buffer](                                        // Lambda-callback kjøres når lesingen er ferdig (eller feiler)
            const boost::system::error_code &ec, size_t /*bytes_transferred*/ ) {
                // Sjekk for IO-feil
                if (ec) {                                                               // Hvis lesing feilet (ec satt), gjør vi ikke noe
                    return;                                                             // - forbindelsen forsvinner når smartpekerene går ut
                }

                // Håndter HTTP-forespørselen
                istream read_stream(read_buffer.get());                             // Pakk bufferen inn i en std::istream for enkel tekstlesing
                string request_line;                                                // Her lagrer vi første linje i HTTP-forespørselen
                getline(read_stream, request_line);                                 // Les første linje: f.eks. "GET / HTTP/1.1"

                if (!request_line.empty() && request_line.back() == '\r')           // Fjern '\r' på slutten for å gjøre om fra RFC 7230
                    request_line.pop_back();                                        // i HTTP-protokollen til C++ vennelig format

                cout << "Request: " << request_line << endl;                        // Logg hva klienten spurte om

                string response;                                                    // Teksten vi skal sende tilbake (inkl. HTTP-headere)

                // Parse hvilken path som ble etterspurt
                // Vanlig request line ser slik ut: "GET /noe HTTP/1.1"
                if (request_line.find("GET / ") == 0 ||                             // Treffer "GET / HTTP/1.1" (med mellomrom etter skråstrek)
                    request_line.find("GET /HTTP") == 0) {                          // Litt grov sjekk som også fanger "GET / HTTP/1.1"
                    response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "Dette er hovedsiden";                                      // Innholdet (body) i responsen
                } else if (request_line.find("GET /en_side") == 0) {
                    response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "Dette er en side";
                } else {
                    response =
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "Error 404: Siden finnes ikke";                             // Svar dersom path ikke gjenkjennes
                }

                auto write_buffer = make_shared<string>(response);                  // Vi må holde på responsteksten til skrivingen 
                                                                                    // er ferdig => bruk shared_ptr 
                                                                                    
                async_write(                                                        // Skriv svaret asynkront til klienten
                    connection->socket,                                             // Hvor vi skriver
                    boost::asio::buffer(*write_buffer),                             // Hva vi skriver
                    [this, connection, write_buffer](
                    const boost::system::error_code &ec, size_t /*bytes_written*/) {
                        if (ec) {                                                       // Hvis det oppstod en feil, gjør vi ingenting mer
                            return;                                                     // fordi socketet blir uansett lukket når obj. dør
                        }
  
                        connection->socket.close();                                     // Når vi er ferdig med å svare, lukker vi forbindelsen
                    }
                );
            
            });
    }

    // Aksepter innkommende tilkoblinger
    void accept() {
        auto connection = make_shared<Connection>(io_context);                  // Lag en Connection for neste klient

        acceptor.async_accept(                                                  // Start accept asynkront for å ikke blokkere under håndteringen
            connection->socket,                                                 // Når noen kobler til, fylles denne socketen
            [this, connection](const boost::system::error_code &ec) {
                accept();                                                       // VELDIG VIKTIG: start en ny accept med en gang,
                                                                                // så vi kan ta imot neste klient parallelt
                if (ec) {                                                           // Ved feil: ignorerer vi bare; ny  
                    return;                                                         //  accept er allerede startet uansett
                }   

                handle_request(connection);                                         // Hvis ingen feil: begynn å lese/serve denne klienten
            }
        );
    }

public:
    // Sett opp endpoint, port 8080 på alle IPv4-adresser
    SimpleHttpServer()
        : endpoint(tcp::v4(), 8080),                                            // tcp::v4() = IPv4; port = 8080
          acceptor(io_context, endpoint) {}                                     // Bind og lytt på endpoint vha. acceptor, 
                                                                                // knyttet til io_context

    void start() {
        accept();                                                               // Begynn å ta imot tilkoblinger

        io_context.run();                                                       // Start eventloopen for å kjøre callbacks
                                                                                // Merk: blokkerer til programmet avsluttes
    }
};

int main() {
    SimpleHttpServer server;                                                    // Lag server-objektet
    cout << "Starting HTTP server on http://localhost:8080"
         << endl;                   
    server.start();                                                             // Start serveren, og blokkerer her til programmet avsluttes
}
