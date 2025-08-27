#include "chessboard.hpp"



// ---------------- ChessBoard::Piece ----------------
ChessBoard::Piece::Piece(Color color) : color(color) {}

ChessBoard::Piece::~Piece() {}

std::string ChessBoard::Piece::color_string() const {
    switch (color) {
        case Color::WHITE:
            return "white";
        case Color::BLACK:
            return "black";
        default:
            throw std::runtime_error("Unknown color");
    }
}



// ---------------- ChessBoard::King ----------------
ChessBoard::King::King(Color color) : Piece(color) {}

std::string ChessBoard::King::type() const {
    return this->color_string() + " king";
}

char ChessBoard::King::algebraic_type() const {
    // Return the algebraic notation character for the king
    return 'K' + (32 * (color == Color::BLACK)); 
}

bool ChessBoard::King::valid_move(int from_x, int from_y, int to_x, int to_y) const {
    // King can move one square in any direction
    return (abs(from_x - to_x) <= 1 && abs(from_y - to_y) <= 1);
}



// ---------------- ChessBoard::Knight ----------------
ChessBoard::Knight::Knight(Color color) : Piece(color) {}

std::string ChessBoard::Knight::type() const {
    return this->color_string() + " knight";
}

char ChessBoard::Knight::algebraic_type() const {
    // Return the algebraic notation character for the knight
    return 'N' + (32 * (color == Color::BLACK));
}

bool ChessBoard::Knight::valid_move(int from_x, int from_y, int to_x, int to_y) const {
    // Knight moves in an L-shape
    return (abs(from_x - to_x) == 2 && abs(from_y - to_y) == 1) ||
           (abs(from_x - to_x) == 1 && abs(from_y - to_y) == 2);
}



// ---------------- ChessBoard ----------------
ChessBoard::ChessBoard() {
    // Initialize 8x8 board
    squares.resize(8);
    for (auto &square_column : squares)
        square_column.resize(8);
}

bool ChessBoard::move_piece(const std::string &from, const std::string &to) {
    int from_x = from[0] - 'a';
    int from_y = std::stoi(std::string() + from[1]) - 1;
    int to_x = to[0] - 'a';
    int to_y = std::stoi(std::string() + to[1]) - 1;

    auto &piece_from = squares[from_x][from_y];
    if (piece_from) {
        if (piece_from->valid_move(from_x, from_y, to_x, to_y)) {
            // std::cout << piece_from->type() << " is moving from " << from << " to " << to << std::endl;
            if (on_piece_move) on_piece_move(*piece_from, from, to);
            
            auto &piece_to = squares[to_x][to_y];
            if (piece_to) {
                if (piece_from->color != piece_to->color) {
                    if (on_piece_removed) on_piece_removed(*piece_to, to);
                    
                    if (auto king = dynamic_cast<King *>(piece_to.get()))
                        if (on_lost_game) on_lost_game(king->color);
                } 
                else {
                    if (on_piece_move_invalid) on_piece_move_invalid(*piece_from, from, to);
                    return false;
                }
            }
            piece_to = move(piece_from);
        
            // After moving, show the updated board
            if (after_piece_move) after_piece_move();

            return true;
        } 
        else {
            if (on_piece_move_invalid) on_piece_move_invalid(*piece_from, from, to);
            return false;
        }
    } 
    else {
        if (on_piece_move_missing) on_piece_move_missing(from);
        return false;
    }
}
