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

using Board = std::array<int, BOARD_ID_SIZE>;

// �Ղ̏��ڂɂ�����2�i���̃t���O�ŕ\������
// �r�b�g��PIECE_TYPE_COUNT�~2��
// ���A���̏��ԂɃt���O������A���̒��ł͏��������ɕ���ł���
//
// ��菬�A��蒆�A����A������A��菬�A��蒆�A����A������

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
