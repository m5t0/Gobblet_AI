#include <gtest/gtest.h>
#include "forward.h"

TEST(board_id_test, test1) {
    // 0,1,2,3
    // 4,5,6
    ASSERT_EQ(board_id(1, 2), 6);
    ASSERT_EQ(board_id(6), std::make_pair(1, 2));
}

TEST(take_pieces_test, test1) {
    // 1011 1101
    ASSERT_EQ(take_pieces(0b1011'1101, 0), 0b1101);
    ASSERT_EQ(take_pieces(0b1011'1101, 1), 0b1011);
}

TEST(convert_to_square_test, test1) {
    ASSERT_EQ(convert_to_square(0b1011, 0), 0b1011);
    ASSERT_EQ(convert_to_square(0b1011, 1), 0b1011'0000);
}

TEST(bits_msb_test, test1) {
    ASSERT_EQ(bits_msb(0b100110), 0b100000);
}

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

TEST(count_board_pieces_test, test1) {
    Board board{
        0,0,0,0b1,
        0,0,0b100,0,
        0,0b1000,0,0,
        0b1'0000,0b10'0000,0b1000'0000,0,
    };

    auto c1 = count_board_pieces(board, 0), c2 = count_board_pieces(board, 1);
    std::array<int, 4> a1{ 1,0,1,1 }, a2{ 1,1,0,1 };
    ASSERT_EQ(c1, a1);
    ASSERT_EQ(c2, a2);
}

TEST(pair_to_board_test, test1) {
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
        0,0,0,0,
        0,0,0,0,
        0,0,0b0000'1111,0b0000'1111,
        0b0000'1111,0b1111'0000,0b1111'0000,0b1111'0000,
    };

    int turn = 0;
    auto p = board_to_pair(board, turn);
    ASSERT_EQ(board, pair_to_board(p));
    auto p1 = board_to_pair(board1, turn);
    ASSERT_EQ(board1, pair_to_board(p1));
    auto p2 = board_to_pair(board2, turn);
    ASSERT_EQ(board2, pair_to_board(p2));
    auto p3 = board_to_pair(board3, turn);
    ASSERT_EQ(board3, pair_to_board(p3));
    auto p4 = board_to_pair(board4, turn);
    ASSERT_EQ(board4, pair_to_board(p4));
    auto p5 = board_to_pair(board5, turn);
    ASSERT_EQ(board5, pair_to_board(p5));
    auto p6 = board_to_pair(board6, turn);
    ASSERT_EQ(board6, pair_to_board(p6));
    auto p7 = board_to_pair(board7, turn);
    ASSERT_EQ(board7, pair_to_board(p7));
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
    auto p0 = board_to_pair(board, turn);
    auto p02 = p0;
    for (int i = 0; i < 4; i++) p0 = rotate_pair(p0);
    ASSERT_EQ(p0, p02);
    auto p1 = board_to_pair(board1, turn);
    auto p12 = p1;
    for (int i = 0; i < 4; i++) p1 = rotate_pair(p1);
    ASSERT_EQ(p1, p12);
    auto p2 = board_to_pair(board2, turn);
    auto p22 = p2;
    for (int i = 0; i < 4; i++) p2 = rotate_pair(p2);
    ASSERT_EQ(p2, p22);
    auto p3 = board_to_pair(board3, turn);
    auto p32 = p3;
    for (int i = 0; i < 4; i++) p3 = rotate_pair(p3);
    ASSERT_EQ(p3, p32);
    auto p4 = board_to_pair(board4, turn);
    auto p42 = p4;
    for (int i = 0; i < 4; i++) p4 = rotate_pair(p4);
    ASSERT_EQ(p4, p42);
    auto p5 = board_to_pair(board5, turn);
    auto p52 = p5;
    for (int i = 0; i < 4; i++) p5 = rotate_pair(p5);
    ASSERT_EQ(p5, p52);
    auto p6 = board_to_pair(board6, turn);
    auto p62 = p6;
    for (int i = 0; i < 4; i++) p6 = rotate_pair(p6);
    ASSERT_EQ(p6, p62);
    auto p7 = board_to_pair(board7, turn);
    auto p72 = p7;
    for (int i = 0; i < 4; i++) p7 = rotate_pair(p7);
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

TEST(transpose_player_pair_test, test1) {
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
    for (int i = 0; i < 2; i++) p0 = transpose_player_pair(p0);
    ASSERT_EQ(p0, p02);
    auto p1 = board_to_pair(board1, turn);
    auto p12 = p1;
    for (int i = 0; i < 2; i++) p1 = transpose_player_pair(p1);
    ASSERT_EQ(p1, p12);
    auto p2 = board_to_pair(board2, turn);
    auto p22 = p2;
    for (int i = 0; i < 2; i++) p2 = transpose_player_pair(p2);
    ASSERT_EQ(p2, p22);
    auto p3 = board_to_pair(board3, turn);
    auto p32 = p3;
    for (int i = 0; i < 2; i++) p3 = transpose_player_pair(p3);
    ASSERT_EQ(p3, p32);
    auto p4 = board_to_pair(board4, turn);
    auto p42 = p4;
    for (int i = 0; i < 2; i++) p4 = transpose_player_pair(p4);
    ASSERT_EQ(p4, p42);
    auto p5 = board_to_pair(board5, turn);
    auto p52 = p5;
    for (int i = 0; i < 2; i++) p5 = transpose_player_pair(p5);
    ASSERT_EQ(p5, p52);
    auto p6 = board_to_pair(board6, turn);
    auto p62 = p6;
    for (int i = 0; i < 2; i++) p6 = transpose_player_pair(p6);
    ASSERT_EQ(p6, p62);
    auto p7 = board_to_pair(board7, turn);
    auto p72 = p7;
    for (int i = 0; i < 2; i++) p7 = transpose_player_pair(p7);
    ASSERT_EQ(p7, p72);
}

TEST(simple_search1_test, test1) {
    //simple_search1();
}

TEST(simple_search2_test, test1) {
    //constexpr int max_depth = 5;
    //int cnt = 0;
    //auto start = std::chrono::system_clock::now();
    //simple_search2(max_depth, cnt);
    //auto end = std::chrono::system_clock::now();
    //auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << t << "ms depth:" << max_depth << ", cnt:" << cnt << std::endl;
}

TEST(simple_search3_test, test1) {
    //constexpr int max_depth = 7;
    //int cnt = 0;
    //auto start = std::chrono::system_clock::now();
    //simple_search3(max_depth, cnt);
    //auto end = std::chrono::system_clock::now();
    //auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << t << "ms depth:" << max_depth << ", cnt:" << cnt << std::endl;
}

TEST(simple_search4_test, test1) {
    constexpr int max_depth = 7;
    int cnt = 0;
    auto start = std::chrono::system_clock::now();
    simple_search4(max_depth, cnt);
    auto end = std::chrono::system_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << t << "ms depth:" << max_depth << ", cnt:" << cnt << std::endl;
}