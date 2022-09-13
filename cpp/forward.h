#ifndef FORWARD_H_
#define FORWARD_H_
#include "cpp.h"

// �Ղ�2�����ł͂Ȃ���1�����ŊǗ�����
// ���ꂼ��̃}�X�ɂ��ċ���邩�ǂ�����2�i���̃t���O�ŊǗ�����
// X���c����, Y��������, ���_�͍���
constexpr inline int BOARD_SIZE = 4;
constexpr inline int BOARD_ID_SIZE = BOARD_SIZE * BOARD_SIZE;

// ��̎�ސ�
constexpr inline int PIECE_TYPE_COUNT = 4;
// �e��ނ��Ƃ̋�̌�
constexpr inline int PIECE_EACH_COUNT = 3;
// �e�X�̐l�̋�̌�
constexpr inline int PIECE_PLAYER_COUNT = PIECE_TYPE_COUNT * PIECE_EACH_COUNT;

using Board = std::array<std::uint32_t, BOARD_ID_SIZE>;

// �Ղ̏��ڂɂ�����2�i���̃t���O�ŕ\������
// �r�b�g��PIECE_TYPE_COUNT�~2��
// ���A���̏��ԂɃt���O������A���̒��ł͏��������ɕ���ł���
//
// ��菬�A��蒆�A����A������A��菬�A��蒆�A����A������

int board_id(int x, int y);
std::pair<int, int> board_id(int id);
std::uint32_t take_pieces(std::uint32_t square, int turn);
std::uint32_t convert_to_square(std::uint32_t square, int turn);
std::optional<bool> check_status(const Board& board);
std::array<int, PIECE_TYPE_COUNT> count_board_pieces(const Board& board, int turn);
Board rotate_board(const Board& p);
Board transpose_board(const Board& p);
Board transpose_player_board(const Board& p);
void simple_search(long long max_depth, long long& cnt, long long& cnt2, std::map<Board, bool>& mp);
boost::multiprecision::cpp_int count_position_wrapper();
boost::multiprecision::cpp_int count_position_wrapper2();
boost::multiprecision::cpp_int count_position_wrapper2_1();
boost::multiprecision::cpp_int count_position_all();

#endif // FORWARD_H_
