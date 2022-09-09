#ifndef FORWARD_H_
#define FORWARD_H_
#include "cpp.h"

// 盤を2次元ではなくて1次元で管理する
// それぞれのマスについて駒があるかどうかを2進数のフラグで管理する
// X軸が縦方向, Y軸が横方向, 原点は左上
constexpr inline int BOARD_SIZE = 4;
constexpr inline int BOARD_ID_SIZE = BOARD_SIZE * BOARD_SIZE;
constexpr inline std::uint64_t BOARD_MASK = (1 << BOARD_ID_SIZE) - 1;

// 駒の種類数
constexpr inline int PIECE_TYPE_COUNT = 4;
// 各種類ごとの駒の個数
constexpr inline int PIECE_EACH_COUNT = 3;
// 各々の人の駒の個数
constexpr inline int PIECE_PLAYER_COUNT = PIECE_TYPE_COUNT * PIECE_EACH_COUNT;

// 先手のボードがfirst, 後手のボードがsecond
// ボード内では小さい順に小、中、大、特大が入っている。
// 16*4=64
using Board = std::pair<std::uint64_t, std::uint64_t>;

std::uint64_t player_board(const Board& board, int turn);
std::uint64_t top_pieces_board(const Board& board, int turn);
std::optional<bool> check_status(const Board& board);
Board flipHorizontal(const Board& x);
Board flipVertical(const Board& x);
Board flipDiagonalA1H8(const Board& x);
Board flipDiagonalA8H1(const Board& x);
Board rotateClockwise90(const Board& x);
Board rotateCounterclockwise90(const Board& x);
Board rotate180(const Board& x);
void debug_board(Board& board, int t);
void simple_search(int max_depth, int& cnt);

#endif // FORWARD_H_
