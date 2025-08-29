#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <functional>


class ChessBoard {
public:
    enum class Color {
        WHITE,
        BLACK
    };

    class Piece {
    public:
        Piece(Color color);
        virtual ~Piece();

        Color color;
        std::string color_string() const;

        /// Return color and type of the chess piece
        virtual std::string type() const = 0;

        /// Return the algebraic notation character for the chess piece
        virtual char algebraic_type() const = 0;

        /// Returns true if the given chess piece move is valid
        virtual bool valid_move(int from_x, int from_y, int to_x, int to_y) const = 0;
    };

    class King : public Piece {
    public:
        King(Color color);

        std::string type() const override;
        char algebraic_type() const override;
        bool valid_move(int from_x, int from_y, int to_x, int to_y) const override;
    };

    class Knight : public Piece {
    public:
        Knight(Color color);

        std::string type() const override;
        char algebraic_type() const override;
        bool valid_move(int from_x, int from_y, int to_x, int to_y) const override;
    };

    ChessBoard();

    /// 8x8 squares occupied by 1 or 0 chess pieces
    std::vector<std::vector<std::unique_ptr<Piece>>> squares;

    /// Function objects for displaying the chessboard
    std::function<void(const Piece &piece, const std::string &from, const std::string &to)> on_piece_move;
    std::function<void(const Piece &piece, const std::string &square)> on_piece_removed;
    std::function<void(Color color)> on_lost_game;
    std::function<void(const Piece &piece, const std::string &from, const std::string &to)> on_piece_move_invalid;
    std::function<void(const std::string &square)> on_piece_move_missing;
    std::function<void()> after_piece_move;

    /// Move a chess piece if it is a valid move.
    /// Does not test for check or checkmate.
    bool move_piece(const std::string &from, const std::string &to);
};

#endif // CHESSBOARD_HPP
