#ifndef FORWARD_H_
#define FORWARD_H_
#include "cpp.h"

// 盤を2次元ではなくて1次元で管理する
// それぞれのマスについて駒があるかどうかを2進数のフラグで管理する
// Xが縦方向, Yが横方向, 原点は左上
constexpr inline int BOARD_SIZE = 4;
constexpr inline int BOARD_ID_SIZE = BOARD_SIZE * BOARD_SIZE;

// 駒の種類数
constexpr inline int PIECE_TYPE_COUNT = 4;
// 各種類ごとの駒の個数
constexpr inline int PIECE_EACH_COUNT = 3;
// 各々の人の駒の個数
constexpr inline int PIECE_PLAYER_COUNT = PIECE_TYPE_COUNT * PIECE_EACH_COUNT;

using Board = std::array<int, BOARD_ID_SIZE>;

// 盤の升目にある駒を2進数のフラグで表現する
// ビットはPIECE_TYPE_COUNT×2個
// 先手、後手の順番にフラグがあり、先手の中では小さい順に並んでいる
//
// 先手小、先手中、先手大、先手特大、後手小、後手中、後手大、後手特大

int board_id(int x, int y);
std::pair<int, int> board_id(int id);
int take_pieces(int square, int turn);
int convert_to_square(int square, int turn);
int bits_msb(int v);
std::optional<bool> check_status(Board& board);
std::array<int, PIECE_TYPE_COUNT> count_board_pieces(Board& board, int turn);
std::pair<long long, long long> board_to_pair(Board& board);
Board pair_to_board(std::pair<long long, long long>& p);
void simple_search1();
void simple_search2(int max_depth,int& cnt);
void simple_search3(int max_depth, int& cnt);

#endif // FORWARD_H_
