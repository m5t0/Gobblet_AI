#include <gtest/gtest.h>
#include "forward.h"

TEST(board_id_test, test1) {
    // 0,1,2,3
    // 4,5,6
    //ASSERT_EQ(board_id(1, 2), 6);
    //ASSERT_EQ(board_id(6), std::make_pair(1, 2));
}

TEST(take_pieces_test, test1) {
    // 1011 1101
    //ASSERT_EQ(take_pieces(0b1011'1101, 0), 0b1101);
    //ASSERT_EQ(take_pieces(0b1011'1101, 1), 0b1011);
}

TEST(convert_to_square_test, test1) {
    //ASSERT_EQ(convert_to_square(0b1011, 0), 0b1011);
    //ASSERT_EQ(convert_to_square(0b1011, 1), 0b1011'0000);
}

TEST(check_status_test, test1) {
    //Board board{
    //    0,0,0,0,
    //    1,1,1,1,
    //    0,0,0,0,
    //    0,0,0,0,
    //};
    //Board board2{
    //    0,0,1,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //};
    //Board board3{
    //    1,0,0,0,
    //    0,1,0,0,
    //    0,0,1,0,
    //    0,0,0,1,
    //};
    //Board board4{
    //    0,0,0,1,
    //    0,0,1,0,
    //    0,1,0,0,
    //    1,0,0,0,
    //};
    //Board board5{
    //    0,0,0,0b1000,
    //    0,0,0b100,0,
    //    0,0b10,0,0,
    //    1,0,0,0,
    //};
    //Board board6{
    //    0,0,0,0b1000'0000,
    //    0,0,0b100'1000,0,
    //    0,0b10'0000,0,0,
    //    0b1'0000,0,0,0,
    //};
    //Board board7{
    //    0,0,0,0b1000'000,
    //    0,0,1,0,
    //    0,0b10'000,0,0,
    //    0b1'000,0,0,0,
    //};
    //ASSERT_EQ(check_status(board), true);
    //ASSERT_EQ(check_status(board2), true);
    //ASSERT_EQ(check_status(board3), true);
    //ASSERT_EQ(check_status(board4), true);
    //ASSERT_EQ(check_status(board5), true);
    //ASSERT_EQ(check_status(board6), std::nullopt);
    //ASSERT_EQ(check_status(board7), std::nullopt);
}

TEST(count_board_pieces_test, test1) {
    //Board board{
    //    0,0,0,0b1,
    //    0,0,0b100,0,
    //    0,0b1000,0,0,
    //    0b1'0000,0b10'0000,0b1000'0000,0,
    //};

    //auto c1 = count_board_pieces(board, 0), c2 = count_board_pieces(board, 1);
    //std::array<int, 4> a1{ 1,0,1,1 }, a2{ 1,1,0,1 };
    //ASSERT_EQ(c1, a1);
    //ASSERT_EQ(c2, a2);
}

TEST(rotate_board_test, test1) {
    //std::vector<Board> boards{
    //{
    //    0,0,0,0b1,
    //    0,0,0b100,0,
    //    0,0b1000,0,0,
    //    0b1'0000,0b10'0000,0b1000'0000,0,
    //},
    //{
    //    0,0,0,0,
    //    1,1,1,0b10,
    //    0,0,0,0,
    //    0,0,0,0,
    //},
    //{
    //    0,0,0b100,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //},
    //{
    //    1,0,0,0,
    //    0,1,0,0,
    //    0,0,0b1000,0,
    //    0,0,0,0b10'0000,
    //},
    //{
    //    0,0,0,1,
    //    0,0,0b1'0000,0,
    //    0,1,0,0,
    //    1,0,0,0,
    //},
    //{
    //    0,0,0,0b1000,
    //    0,0,0b100,0,
    //    0,0b10,0,0,
    //    1,0,0,0,
    //},
    //{
    //    0,0,0,0b1000'0000,
    //    0,0,0b100'0000,0,
    //    0,0b10'0000,0,0,
    //    0b1'0000,0,0,0,
    //},
    //{
    //    0,0,0,0b1000'0000,
    //    0,0,1,0,
    //    0,0b10'0000,0,0,
    //    0b1'0000,0,0,0,
    //}
    //};

    //int turn = 0;
    //for (int i = 0; i < boards.size(); i++) {
    //    auto b = boards[i];
    //    for (int j = 0; j < 4; j++) b = rotate_board(b);
    //    ASSERT_EQ(b, boards[i]);
    //}
}

TEST(transpose_board_test, test1) {
    //std::vector<Board> boards{
    //{
    //    0,0,0,0b1,
    //    0,0,0b100,0,
    //    0,0b1000,0,0,
    //    0b1'0000,0b10'0000,0b1000'0000,0,
    //},
    //{
    //    0,0,0,0,
    //    1,1,1,0b10,
    //    0,0,0,0,
    //    0,0,0,0,
    //},
    //{
    //    0,0,0b100,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //    0,0,1,0,
    //},
    //{
    //    1,0,0,0,
    //    0,1,0,0,
    //    0,0,0b1000,0,
    //    0,0,0,0b10'0000,
    //},
    //{
    //    0,0,0,1,
    //    0,0,0b1'0000,0,
    //    0,1,0,0,
    //    1,0,0,0,
    //},
    //{
    //    0,0,0,0b1000,
    //    0,0,0b100,0,
    //    0,0b10,0,0,
    //    1,0,0,0,
    //},
    //{
    //    0,0,0,0b1000'0000,
    //    0,0,0b100'0000,0,
    //    0,0b10'0000,0,0,
    //    0b1'0000,0,0,0,
    //},
    //{
    //    0,0,0,0b1000'0000,
    //    0,0,1,0,
    //    0,0b10'0000,0,0,
    //    0b1'0000,0,0,0,
    //}
    //};

    //int turn = 0;
    //for (int i = 0; i < boards.size(); i++) {
    //    auto b = boards[i];
    //    for (int j = 0; j < 2; j++) b = transpose_board(b);
    //    ASSERT_EQ(b, boards[i]);
    //}
}

TEST(transpose_player_board_test, test1) {
    //std::vector<Board> boards{
    //    {
    //        0,0,0,0b1,
    //        0,0,0b100,0,
    //        0,0b1000,0,0,
    //        0b1'0000,0b10'0000,0b1000'0000,0,
    //    },
    //    {
    //        0,0,0,0,
    //        1,1,1,0b10,
    //        0,0,0,0,
    //        0,0,0,0,
    //    },
    //    {
    //        0,0,0b100,0,
    //        0,0,1,0,
    //        0,0,1,0,
    //        0,0,1,0,
    //    },
    //    {
    //        1,0,0,0,
    //        0,1,0,0,
    //        0,0,0b1000,0,
    //        0,0,0,0b10'0000,
    //    },
    //    {
    //        0,0,0,1,
    //        0,0,0b1'0000,0,
    //        0,1,0,0,
    //        1,0,0,0,
    //    },
    //    {
    //        0,0,0,0b1000,
    //        0,0,0b100,0,
    //        0,0b10,0,0,
    //        1,0,0,0,
    //    },
    //    {
    //        0,0,0,0b1000'0000,
    //        0,0,0b100'0000,0,
    //        0,0b10'0000,0,0,
    //        0b1'0000,0,0,0,
    //    },
    //    {
    //        0,0,0,0b1000'0000,
    //        0,0,1,0,
    //        0,0b10'0000,0,0,
    //        0b1'0000,0,0,0,
    //    }
    //};

    //int turn = 0;
    //for (int i = 0; i < boards.size(); i++) {
    //    auto b = boards[i];
    //    for (int j = 0; j < 4; j++) b = transpose_player_board(b);
    //    ASSERT_EQ(b, boards[i]);
    //}
}

TEST(simple_search_test, test1) {
    //constexpr int max_depth = 8;
    //long long cnt = 0, cnt2 = 0;
    //std::map<Board, bool> mp;
    //simple_search(max_depth, cnt, cnt2, mp);
    //std::cout << "depth:" << max_depth << ", cnt:" << cnt << ", cnt2:" << cnt2 << ", mp size:" << mp.size() << ", mp maxsize:" << mp.max_size() << std::endl;
}

TEST(count_position_wrapper_test, test1) {
    auto cnt = count_position_wrapper();
    std::cout << "cnt:" << cnt << std::endl;
}

TEST(count_position_wrapper2_test, test1) {
    //auto cnt = count_position_wrapper2();
    //std::cout << "cnt:" << cnt << std::endl;
}

TEST(count_position_wrapper3_test, test1) {
    //auto res = count_position_wrapper3();
    //
    //cpp_int sum = 0;
    //for (auto& [p, v] : res) sum += v;
    //std::cout << "cnt:" << sum << std::endl;

    //std::ofstream ofst(std::format("../../../../output/position_count_{}.txt", BOARD_SIZE));

    //for (auto& [piece_list, value] : res) {
    //    for (auto& pl : piece_list) ofst << pl << " ";
    //    ofst << value << std::endl;
    //}
}

TEST(possible_transition_phase_test, test1) {
    //auto res = possible_transition_phase();
    //std::array<int, 2 * PIECE_TYPE_COUNT> idx{};

    //for (auto& [piece_list, value] : res) {
    //    for (auto& pl : piece_list) std::cout << pl << " ";
    //    std::cout << value << std::endl;
    //    if (piece_list[4] != 0) break;
    //}
}

TEST(possible_transition_phase2_test, test1) {
    //auto [res, res2] = possible_transition_phase2();
    //std::array<int, 2 * PIECE_TYPE_COUNT> idx{};

    //for (auto& [piece_list, value] : res) {
    //    for (auto& pl : piece_list) std::cout << pl << " ";
    //    std::cout << value << std::endl;
    //    if (piece_list[4] != 0) break;
    //}
}

TEST(possible_transition_phase2_test, test2) {
    //auto [res, res2] = possible_transition_phase2();
    //std::array<int, 2 * PIECE_TYPE_COUNT> idx{};
    //std::ofstream ofst(std::format("../../../../output/possible_transition_phase_{}.txt", BOARD_SIZE));

    //std::cout << "sum:" << res.begin()->second << std::endl;
    //for (auto& [piece_list, value] : res) {
    //    for (auto& pl : piece_list) ofst << pl << " ";
    //    ofst << value << std::endl;
    //}
}

TEST(possible_transition_phase2_test, test3) {
    //auto [res, res2] = possible_transition_phase2();
    //std::array<int, 2 * PIECE_TYPE_COUNT> idx{};
    //std::ofstream ofst(std::format("../../../../output/possible_transition_phase_p_{}.txt", BOARD_SIZE));

    //auto sum = res.begin()->second;
    //std::cout << "sum:" << sum << std::endl;
    //for (auto& [piece_list, value] : res) {
    //    auto res = cpp_dec_float_50(value) / cpp_dec_float_50(sum);
    //    if (res < 1e-14) continue;

    //    for (auto& pl : piece_list) ofst << pl << " ";
    //    ofst << res << " " << res2[piece_list] << std::endl;
    //}
}

TEST(possible_transition_phase2_test, test4) {
    //auto [res, res2] = possible_transition_phase2();
    //std::array<int, 2 * PIECE_TYPE_COUNT> idx{};

    //std::ofstream ofst(std::format("../../../../output/possible_transition_phase_p_{}.csv", BOARD_SIZE));

    //for (int i = 0; i < PIECE_TYPE_COUNT; i++) ofst << std::format("first_player_{}", i + 1) << ",";
    //for (int i = 0; i < PIECE_TYPE_COUNT; i++) ofst << std::format("second_player_{}", i + 1) << ",";
    //ofst << "proportion,count" << std::endl;

    //auto sum = res.begin()->second;
    //std::cout << "sum:" << sum << std::endl;
    //for (auto& [piece_list, value] : res) {
    //    auto res = cpp_dec_float_50(value) / cpp_dec_float_50(sum);
    //    if (res < 1e-30) continue;

    //    for (auto& pl : piece_list) ofst << pl << ",";
    //    ofst << res << "," << res2[piece_list] << std::endl;
    //}
}

TEST(count_position_all_test, test1) {
    //auto cnt = count_position_all();
    //std::cout << "cnt:" << cnt << std::endl;
}
