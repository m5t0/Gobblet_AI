#include <gtest/gtest.h>
#include "forward.h"

std::array<Board, 7> boards{
std::make_pair(
    0b0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
    0b0000'0000'0000'0000'0000'0000'0000'0000'0100'0000'0000'0010'0000'0000'0000'0000
),
std::make_pair(
    0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0100'0100'0100'0100,
    0b0000'0000'0000'0010'0000'0000'0000'0000'0000'1000'0000'0000'0000'0000'0000'0000
),
std::make_pair(
    0b0000'0000'0000'0000'0000'0000'0000'0000'1000'0100'0010'0001'0000'0000'0000'0000,
    0b0000'0000'0000'0000'0000'0000'0000'0000'0000'1000'0000'0000'0100'0000'0000'0000
),
std::make_pair(
    0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1000'0000'0010'0000,
    0b0000'0000'0000'0000'0001'0010'0100'1000'0000'0000'0000'0000'0000'0000'0000'0000
),
std::make_pair(
    0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1000'0000'0010'0000,
    0b1000'0000'0000'0000'0000'0100'0000'0000'0000'0000'0010'0000'0000'0000'0000'0001
),
std::make_pair(
    0b0001'0000'0000'0000'0000'0010'0000'0000'0000'0000'0100'0000'0000'0000'0000'1000,
    0b1000'0000'0000'0000'0000'0100'0000'0000'0000'0000'0010'0000'0000'0000'0000'0100
),
std::make_pair(
    0b0001'0000'0000'0000'0010'0010'0000'0000'0000'0000'0100'0000'1000'0000'0000'0000,
    0b1000'0000'1000'0000'0000'0100'0000'1000'0000'0000'0010'0000'0000'0000'0000'0100
)
};

void print_board(Board& b) {
    std::cout << "first" << std::endl;
    for (int k = 0; k < BOARD_SIZE; k++) {
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            std::cout << std::bitset<BOARD_SIZE>(b.first >> (BOARD_ID_SIZE * t + BOARD_SIZE * k)) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "second" << std::endl;
    for (int k = 0; k < BOARD_SIZE; k++) {
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            std::cout << std::bitset<BOARD_SIZE>(b.first >> (BOARD_ID_SIZE * t + BOARD_SIZE * k)) << " ";
        }
        std::cout << std::endl;
    }
}

TEST(check_status_test, test1) {
    std::array<std::optional<bool>, 8> ans{ true, true, true, false, false, true, std::nullopt };
    for (int i = 0; i < boards.size(); i++) ASSERT_EQ(check_status(boards[i]), ans[i]);
}

TEST(flipHorizontal_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 2; j++) {
            print_board(b);
            b = flipHorizontal(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(flipVertical_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 2; j++) {
            print_board(b);
            b = flipVertical(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(flipDiagonalA1H8_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 2; j++) {
            print_board(b);
            b = flipDiagonalA1H8(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(flipDiagonalA8H1_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 2; j++) {
            print_board(b);
            b = flipDiagonalA8H1(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(rotateClockwise90_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 4; j++) {
            print_board(b);
            b = rotateClockwise90(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(rotateCounterclockwise90_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 4; j++) {
            print_board(b);
            b = rotateCounterclockwise90(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(rotate180_test, test1) {
    int turn = 0;
    for (int i = 0; i < boards.size(); i++) {
        auto b = boards[i];
        for (int j = 0; j < 4; j++) {
            print_board(b);
            b = rotate180(b);
        }
        ASSERT_EQ(boards[i], b);
    }
}

TEST(simple_search_test, test1) {
    constexpr int max_depth = 8;
    int cnt = 0;
    auto start = std::chrono::system_clock::now();
    simple_search(max_depth, cnt);
    auto end = std::chrono::system_clock::now();
    auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << t << "ms depth:" << max_depth << ", cnt:" << cnt << std::endl;
}