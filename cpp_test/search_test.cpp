#include <gtest/gtest.h>
#include "forward.h"



TEST(check_status_test, test1) {
    Board board{
        0,0,0,0,
        1,1,1,1,
        0,0,0,0,
        0,0,0,0,
    };
    Board board2{
        0,0,1,0,
        0,0,1,0,
        0,0,1,0,
        0,0,1,0,
    };
    Board board3{
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1,
    };
    Board board4{
        0,0,0,1,
        0,0,1,0,
        0,1,0,0,
        1,0,0,0,
    };
    Board board5{
        0,0,0,0b1000,
        0,0,0b100,0,
        0,0b10,0,0,
        1,0,0,0,
    };
    Board board6{
        0,0,0,0b1000'0000,
        0,0,0b100'0000,0,
        0,0b10'0000,0,0,
        0b1'0000,0,0,0,
    };
    Board board7{
        0,0,0,0b1000'000,
        0,0,1,0,
        0,0b10'000,0,0,
        0b1'000,0,0,0,
    };
    ASSERT_EQ(check_status(board), true);
    ASSERT_EQ(check_status(board2), true);
    ASSERT_EQ(check_status(board3), true);
    ASSERT_EQ(check_status(board4), true);
    ASSERT_EQ(check_status(board5), true);
    ASSERT_EQ(check_status(board6), false);
    ASSERT_EQ(check_status(board7), std::nullopt);
}
TEST(rotate_pair_test, test1) {
    Board board{
        0,0,0,0b1,
        0,0,0b100,0,
        0,0b1000,0,0,
        0b1'0000,0b10'0000,0b1000'0000,0,
    };
    Board board1{
        0,0,0,0,
        1,1,1,0b10,
        0,0,0,0,
        0,0,0,0,
    };
    Board board2{
        0,0,0b100,0,
        0,0,1,0,
        0,0,1,0,
        0,0,1,0,
    };
    Board board3{
        1,0,0,0,
        0,1,0,0,
        0,0,0b1000,0,
        0,0,0,0b10'0000,
    };
    Board board4{
        0,0,0,1,
        0,0,0b1'0000,0,
        0,1,0,0,
        1,0,0,0,
    };
    Board board5{
        0,0,0,0b1000,
        0,0,0b100,0,
        0,0b10,0,0,
        1,0,0,0,
    };
    Board board6{
        0,0,0,0b1000'0000,
        0,0,0b100'0000,0,
        0,0b10'0000,0,0,
        0b1'0000,0,0,0,
    };
    Board board7{
        0,0,0,0b1000'0000,
        0,0,1,0,
        0,0b10'0000,0,0,
        0b1'0000,0,0,0,
    };

    int turn = 0;
    auto p0 = board;
    auto p02 = p0;
    for (int i = 0; i < 4; i++) p0 = rotateClockwise90(p0);
    ASSERT_EQ(p0, p02);
    auto p1 = board1;
    auto p12 = p1;
    for (int i = 0; i < 4; i++) p1 = rotateClockwise90(p1);
    ASSERT_EQ(p1, p12);
    auto p2 = board2;
    auto p22 = p2;
    for (int i = 0; i < 4; i++) p2 = rotateClockwise90(p2);
    ASSERT_EQ(p2, p22);
    auto p3 = board3;
    auto p32 = p3;
    for (int i = 0; i < 4; i++) p3 = rotateClockwise90(p3);
    ASSERT_EQ(p3, p32);
    auto p4 = board4;
    auto p42 = p4;
    for (int i = 0; i < 4; i++) p4 = rotateClockwise90(p4);
    ASSERT_EQ(p4, p42);
    auto p5 = board5;
    auto p52 = p5;
    for (int i = 0; i < 4; i++) p5 = rotateClockwise90(p5);
    ASSERT_EQ(p5, p52);
    auto p6 = board6;
    auto p62 = p6;
    for (int i = 0; i < 4; i++) p6 = rotateClockwise90(p6);
    ASSERT_EQ(p6, p62);
    auto p7 = board7;
    auto p72 = p7;
    for (int i = 0; i < 4; i++) p7 = rotateClockwise90(p7);
    ASSERT_EQ(p7, p72);
}

TEST(transpose_pair_test, test1) {
    Board board{
        0,0,0,0b1,
        0,0,0b100,0,
        0,0b1000,0,0,
        0b1'0000,0b10'0000,0b1000'0000,0,
    };
    Board board1{
        0,0,0,0,
        1,1,1,0b10,
        0,0,0,0,
        0,0,0,0,
    };
    Board board2{
        0,0,0b100,0,
        0,0,1,0,
        0,0,1,0,
        0,0,1,0,
    };
    Board board3{
        1,0,0,0,
        0,1,0,0,
        0,0,0b1000,0,
        0,0,0,0b10'0000,
    };
    Board board4{
        0,0,0,1,
        0,0,0b1'0000,0,
        0,1,0,0,
        1,0,0,0,
    };
    Board board5{
        0,0,0,0b1000,
        0,0,0b100,0,
        0,0b10,0,0,
        1,0,0,0,
    };
    Board board6{
        0,0,0,0b1000'0000,
        0,0,0b100'0000,0,
        0,0b10'0000,0,0,
        0b1'0000,0,0,0,
    };
    Board board7{
        0,0,0,0b1000'0000,
        0,0,1,0,
        0,0b10'0000,0,0,
        0b1'0000,0,0,0,
    };

    int turn = 0;
    auto p0 = board_to_pair(board, turn);
    auto p02 = p0;
    for (int i = 0; i < 2; i++) p0 = transpose_pair(p0);
    ASSERT_EQ(p0, p02);
    auto p1 = board_to_pair(board1, turn);
    auto p12 = p1;
    for (int i = 0; i < 2; i++) p1 = transpose_pair(p1);
    ASSERT_EQ(p1, p12);
    auto p2 = board_to_pair(board2, turn);
    auto p22 = p2;
    for (int i = 0; i < 2; i++) p2 = transpose_pair(p2);
    ASSERT_EQ(p2, p22);
    auto p3 = board_to_pair(board3, turn);
    auto p32 = p3;
    for (int i = 0; i < 2; i++) p3 = transpose_pair(p3);
    ASSERT_EQ(p3, p32);
    auto p4 = board_to_pair(board4, turn);
    auto p42 = p4;
    for (int i = 0; i < 2; i++) p4 = transpose_pair(p4);
    ASSERT_EQ(p4, p42);
    auto p5 = board_to_pair(board5, turn);
    auto p52 = p5;
    for (int i = 0; i < 2; i++) p5 = transpose_pair(p5);
    ASSERT_EQ(p5, p52);
    auto p6 = board_to_pair(board6, turn);
    auto p62 = p6;
    for (int i = 0; i < 2; i++) p6 = transpose_pair(p6);
    ASSERT_EQ(p6, p62);
    auto p7 = board_to_pair(board7, turn);
    auto p72 = p7;
    for (int i = 0; i < 2; i++) p7 = transpose_pair(p7);
    ASSERT_EQ(p7, p72);
}

//TEST(transpose_player_pair_test, test1) {
//    Board board{
//        0,0,0,0b1,
//        0,0,0b100,0,
//        0,0b1000,0,0,
//        0b1'0000,0b10'0000,0b1000'0000,0,
//    };
//    Board board1{
//        0,0,0,0,
//        1,1,1,0b10,
//        0,0,0,0,
//        0,0,0,0,
//    };
//    Board board2{
//        0,0,0b100,0,
//        0,0,1,0,
//        0,0,1,0,
//        0,0,1,0,
//    };
//    Board board3{
//        1,0,0,0,
//        0,1,0,0,
//        0,0,0b1000,0,
//        0,0,0,0b10'0000,
//    };
//    Board board4{
//        0,0,0,1,
//        0,0,0b1'0000,0,
//        0,1,0,0,
//        1,0,0,0,
//    };
//    Board board5{
//        0,0,0,0b1000,
//        0,0,0b100,0,
//        0,0b10,0,0,
//        1,0,0,0,
//    };
//    Board board6{
//        0,0,0,0b1000'0000,
//        0,0,0b100'0000,0,
//        0,0b10'0000,0,0,
//        0b1'0000,0,0,0,
//    };
//    Board board7{
//        0,0,0,0b1000'0000,
//        0,0,1,0,
//        0,0b10'0000,0,0,
//        0b1'0000,0,0,0,
//    };
//
//    int turn = 0;
//    auto p0 = board_to_pair(board, turn);
//    auto p02 = p0;
//    for (int i = 0; i < 2; i++) p0 = transpose_player_pair(p0);
//    ASSERT_EQ(p0, p02);
//    auto p1 = board_to_pair(board1, turn);
//    auto p12 = p1;
//    for (int i = 0; i < 2; i++) p1 = transpose_player_pair(p1);
//    ASSERT_EQ(p1, p12);
//    auto p2 = board_to_pair(board2, turn);
//    auto p22 = p2;
//    for (int i = 0; i < 2; i++) p2 = transpose_player_pair(p2);
//    ASSERT_EQ(p2, p22);
//    auto p3 = board_to_pair(board3, turn);
//    auto p32 = p3;
//    for (int i = 0; i < 2; i++) p3 = transpose_player_pair(p3);
//    ASSERT_EQ(p3, p32);
//    auto p4 = board_to_pair(board4, turn);
//    auto p42 = p4;
//    for (int i = 0; i < 2; i++) p4 = transpose_player_pair(p4);
//    ASSERT_EQ(p4, p42);
//    auto p5 = board_to_pair(board5, turn);
//    auto p52 = p5;
//    for (int i = 0; i < 2; i++) p5 = transpose_player_pair(p5);
//    ASSERT_EQ(p5, p52);
//    auto p6 = board_to_pair(board6, turn);
//    auto p62 = p6;
//    for (int i = 0; i < 2; i++) p6 = transpose_player_pair(p6);
//    ASSERT_EQ(p6, p62);
//    auto p7 = board_to_pair(board7, turn);
//    auto p72 = p7;
//    for (int i = 0; i < 2; i++) p7 = transpose_player_pair(p7);
//    ASSERT_EQ(p7, p72);
//}
//
TEST(simple_search_test, test1) {
    constexpr int max_depth = 5;
    int cnt = 0;
    auto start = std::chrono::system_clock::now();
    simple_search(max_depth, cnt);
    auto end = std::chrono::system_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << t << "ms depth:" << max_depth << ", cnt:" << cnt << std::endl;
}