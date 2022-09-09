#ifndef FORWARD_H_
#define FORWARD_H_
#include "cpp.h"

// �Ղ�2�����ł͂Ȃ���1�����ŊǗ�����
// ���ꂼ��̃}�X�ɂ��ċ���邩�ǂ�����2�i���̃t���O�ŊǗ�����
// X�����c����, Y����������, ���_�͍���
constexpr inline int BOARD_SIZE = 4;
constexpr inline int BOARD_ID_SIZE = BOARD_SIZE * BOARD_SIZE;
constexpr inline std::uint64_t BOARD_MASK = (1 << BOARD_ID_SIZE) - 1;

// ��̎�ސ�
constexpr inline int PIECE_TYPE_COUNT = 4;
// �e��ނ��Ƃ̋�̌�
constexpr inline int PIECE_EACH_COUNT = 3;
// �e�X�̐l�̋�̌�
constexpr inline int PIECE_PLAYER_COUNT = PIECE_TYPE_COUNT * PIECE_EACH_COUNT;

// ���̃{�[�h��first, ���̃{�[�h��second
// �{�[�h���ł͏��������ɏ��A���A��A���傪�����Ă���B
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
