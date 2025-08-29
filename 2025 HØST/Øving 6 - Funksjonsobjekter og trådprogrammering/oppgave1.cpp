#include "src/chessboard.hpp"


class ChessBoardPrint {
private:
    ChessBoard* board;
public:
    ChessBoardPrint(ChessBoard* board);
};

ChessBoardPrint::ChessBoardPrint(ChessBoard* board) : board(board) {
    board->on_piece_move = [](const ChessBoard::Piece& piece, const std::string& from, const std::string& to) {
        std::cout << piece.type() << " is moving from " << from << " to " << to << std::endl;
    };

    board->on_piece_removed = [](const ChessBoard::Piece& piece, const std::string& square) {
        std::cout << piece.type() << " is being removed from " << square << std::endl;
    };

    board->on_lost_game = [](ChessBoard::Color color) {
        if (color == ChessBoard::Color::WHITE)  std::cout << "Black";
        else                                    std::cout << "White";
        
        std::cout << " won the game" << std::endl;
    };

    board->on_piece_move_invalid = [](const ChessBoard::Piece &piece, const std::string &from, const std::string &to) {
        std::cout << "can not move " << piece.type() << " from " << from << " to " << to << std::endl;
    };

    board->on_piece_move_missing = [](const std::string& from) {
        std::cout << "no piece at " << from << std::endl;
    };

    board->after_piece_move = [board]() {
        std::cout << "Board after move:" << std::endl;
        for (int y = 7; y >= 0; --y) {
            for (int x = 0; x < 8; ++x) {
                if (board->squares[x][y]) {
                    std::cout << board->squares[x][y]->algebraic_type() << " ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };
}




int main() {
    ChessBoard board;
    ChessBoardPrint print(&board);

    board.squares[4][0] = std::make_unique<ChessBoard::King>(ChessBoard::Color::WHITE);
    board.squares[1][0] = std::make_unique<ChessBoard::Knight>(ChessBoard::Color::WHITE);
    board.squares[6][0] = std::make_unique<ChessBoard::Knight>(ChessBoard::Color::WHITE);

    board.squares[4][7] = std::make_unique<ChessBoard::King>(ChessBoard::Color::BLACK);
    board.squares[1][7] = std::make_unique<ChessBoard::Knight>(ChessBoard::Color::BLACK);
    board.squares[6][7] = std::make_unique<ChessBoard::Knight>(ChessBoard::Color::BLACK);

    std::cout << "Invalid moves:" << std::endl;
    board.move_piece("e3", "e2");
    board.move_piece("e1", "e3");
    board.move_piece("b1", "b2");
    std::cout << std::endl;

    std::cout << "A simulated game:" << std::endl;
    board.move_piece("e1", "e2");
    board.move_piece("g8", "h6");
    board.move_piece("b1", "c3");
    board.move_piece("h6", "g8");
    board.move_piece("c3", "d5");
    board.move_piece("g8", "h6");
    board.move_piece("d5", "f6");
    board.move_piece("h6", "g8");
    board.move_piece("f6", "e8");
}