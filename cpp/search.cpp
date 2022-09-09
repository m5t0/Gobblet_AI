// count.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "forward.h"
#include "cpp.h"

// 手番の人の駒が存在するかどうかのボードに変換する
std::uint64_t player_board(const Board& board, int turn) {
    assert(turn == 0 || turn == 1);
    if (turn == 0) return (board.first | (board.first >> BOARD_ID_SIZE) | (board.first >> (2 * BOARD_ID_SIZE)) | (board.first >> (3 * BOARD_ID_SIZE))) & BOARD_MASK;
    else return (board.second | (board.second >> BOARD_ID_SIZE) | (board.second >> (2 * BOARD_ID_SIZE)) | (board.second >> (3 * BOARD_ID_SIZE))) & BOARD_MASK;
}

// 相手の駒が上にない駒だけがboardにあるように変換する
std::uint64_t top_pieces_board(const Board& board, int turn) {
    assert(turn == 0 || turn == 1);
    if (turn == 0) return board.first & (~(board.second >> BOARD_ID_SIZE)) & (~(board.second >> (2 * BOARD_ID_SIZE))) & (~(board.second >> (3 * BOARD_ID_SIZE)));
    else return board.second & (~(board.first >> BOARD_ID_SIZE)) & (~(board.first >> (2 * BOARD_ID_SIZE))) & (~(board.first >> (3 * BOARD_ID_SIZE)));
}

// 先手が勝ったらtrue, 負けたらfalse
// 勝ち負けが決まらない場合nullopt
std::optional<bool> check_status(const Board& board) {
    assert((board.first & board.second) == 0);
    auto first = top_pieces_board(board, 0), second = top_pieces_board(board, 1);
    first = (first | (first >> BOARD_ID_SIZE) | (first >> (2 * BOARD_ID_SIZE)) | (first >> (3 * BOARD_ID_SIZE))) & BOARD_MASK;
    second = (second | (second >> BOARD_ID_SIZE) | (second >> (2 * BOARD_ID_SIZE)) | (second >> (3 * BOARD_ID_SIZE))) & BOARD_MASK;

    // 縦
    constexpr std::uint64_t column_mask = 0b0001'0001'0001'0001;
    if (std::popcount(first & column_mask) == BOARD_SIZE
        || std::popcount(first & (column_mask << 1)) == BOARD_SIZE
        || std::popcount(first & (column_mask << 2)) == BOARD_SIZE
        || std::popcount(first & (column_mask << 3)) == BOARD_SIZE) return true;
    if (std::popcount(second & column_mask) == BOARD_SIZE
        || std::popcount(second & (column_mask << 1)) == BOARD_SIZE
        || std::popcount(second & (column_mask << 2)) == BOARD_SIZE
        || std::popcount(second & (column_mask << 3)) == BOARD_SIZE) return false;

    // 横
    constexpr std::uint64_t row_mask = 0b1111;
    if (std::popcount(first & row_mask) == BOARD_SIZE
        || std::popcount(first & (row_mask << BOARD_SIZE)) == BOARD_SIZE
        || std::popcount(first & (row_mask << (2 * BOARD_SIZE))) == BOARD_SIZE
        || std::popcount(first & (row_mask << (3 * BOARD_SIZE))) == BOARD_SIZE) return true;
    if (std::popcount(second & row_mask) == BOARD_SIZE
        || std::popcount(second & (row_mask << BOARD_SIZE)) == BOARD_SIZE
        || std::popcount(second & (row_mask << (2 * BOARD_SIZE))) == BOARD_SIZE
        || std::popcount(second & (row_mask << (3 * BOARD_SIZE))) == BOARD_SIZE) return false;

    // 斜め
    constexpr std::uint64_t diagonal_mask = 0b1000'0100'0010'0001;
    constexpr std::uint64_t diagonal_mask2 = 0b0001'0010'0100'1000;
    if (std::popcount(first & diagonal_mask) == BOARD_SIZE || std::popcount(first & diagonal_mask2) == BOARD_SIZE) return true;
    if (std::popcount(second & diagonal_mask) == BOARD_SIZE || std::popcount(second & diagonal_mask2) == BOARD_SIZE) return false;

    return std::nullopt;
}

std::uint64_t delta_swap(std::uint64_t x, std::uint64_t mask, std::uint64_t delta) {
    std::uint64_t t = (x ^ (x >> delta)) & mask;
    return x ^ t ^ (t << delta);
}

// 水平方向をflipする
std::uint64_t flipHorizontal(std::uint64_t x) {
    x = delta_swap(x, 0x5555555555555555, 1);
    return delta_swap(x, 0x3333333333333333, 2);
}
Board flipHorizontal(const Board& board) { return { flipHorizontal(board.first), flipHorizontal(board.second) }; }

// 垂直方向をflipする
std::uint64_t flipVertical(std::uint64_t x) {
    x = delta_swap(x, 0xF0F0F0F0F0F0F0F, 4);
    return delta_swap(x, 0x00FF00FF00FF00FF, 8);
}
Board flipVertical(const Board& board) { return { flipVertical(board.first), flipVertical(board.second) }; }

// 左上から右下の対角線に沿ってflip
std::uint64_t flipDiagonalA1H8(std::uint64_t x) {
    x = delta_swap(x, 0xA0A0A0A0A0A0A0A, 3);
    return delta_swap(x, 0xCC00CC00CC00CC, 6);
}
Board flipDiagonalA1H8(const Board& board) { return { flipDiagonalA1H8(board.first), flipDiagonalA1H8(board.second) }; }

// 右上から左下の対角線に沿ってflip
std::uint64_t flipDiagonalA8H1(std::uint64_t x) {
    x = delta_swap(x, 0x505050505050505, 5);
    return delta_swap(x, 0x33003300330033, 10);
}
Board flipDiagonalA8H1(const Board& board) { return { flipDiagonalA8H1(board.first), flipDiagonalA8H1(board.second) }; }

// 時計回りに90度回転
std::uint64_t rotateClockwise90(std::uint64_t x) {
    return flipHorizontal(flipDiagonalA1H8(x));
}
Board rotateClockwise90(const Board& board) { return { rotateClockwise90(board.first), rotateClockwise90(board.second) }; }

// 反時計回りに90度回転
std::uint64_t rotateCounterclockwise90(std::uint64_t x) {
    return flipVertical(flipDiagonalA1H8(x));
}
Board rotateCounterclockwise90(const Board& board) { return { rotateCounterclockwise90(board.first), rotateCounterclockwise90(board.second) }; }

// 180度回転
std::uint64_t rotate180(std::uint64_t x) {
    return flipVertical(flipHorizontal(x));
}
Board rotate180(const Board& board) { return { rotate180(board.first), rotate180(board.second) }; }


// hashの独自実装
class my_hash {
public:
    size_t operator()(const std::pair<std::uint64_t, std::uint64_t>& p) const {
        auto hash1 = std::hash<std::uint64_t>{}(p.first);
        auto hash2 = std::hash<std::uint64_t>{}(p.second);

        size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};


// ボードをゆっくり出力する
// 手番の人のビットが右に表示されることに注意
// tの単位はms
void debug_board(const Board& board, int t) {
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        std::int64_t b = 0;
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            b |= ((board.first >> (t * BOARD_ID_SIZE + id)) & 1) << t;
        }
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            b |= ((board.second >> (t * BOARD_ID_SIZE + id)) & 1) << (PIECE_TYPE_COUNT + t);
        }

        std::cout << std::bitset<2 * PIECE_TYPE_COUNT>(b);
        if (id % BOARD_SIZE == BOARD_SIZE - 1) std::cout << std::endl;
        else std::cout << " ";
    }
    std::cout << std::endl << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}


// メモ化有り深さ優先探索
// 対称な局面は同一視する
// 手番の人のボードがfirstに来るようにする。
void depth_search(Board& board, int depth, int& cnt, std::map<std::pair<std::uint64_t, std::uint64_t>, bool>& mp) {
    cnt += 1;
    if (depth == 0) return;
    {
        Board hp{ std::min({
            board,
            flipHorizontal(board),
            flipVertical(board),
            flipDiagonalA1H8(board),
            flipDiagonalA8H1(board),
            rotateClockwise90(board),
            rotateCounterclockwise90(board),
            rotate180(board)
            }) };

        if (mp.find(hp) != mp.end()) return;
        mp[hp] = true;
    }

    // 終了局面かどうかを確認する
    auto result = check_status(board);
    if (result) return;

    //debug_board(board, 1000);

    // 先手がsecond, 後手がfirstにする。
    std::swap(board.first, board.second);
    depth -= 1;

    // 盤上で駒を動かす
    {
        auto top_pieces = top_pieces_board(board, 1);
        top_pieces = top_pieces & ((~top_pieces) >> BOARD_ID_SIZE) & ((~top_pieces) >> (2 * BOARD_ID_SIZE)) & ((~top_pieces) >> (3 * BOARD_ID_SIZE));
        auto block_pieces = board.first | (board.first >> BOARD_ID_SIZE) | (board.first >> (2 * BOARD_ID_SIZE)) | (board.first >> (3 * BOARD_ID_SIZE))
            | board.second | (board.second >> BOARD_ID_SIZE) | (board.second >> (2 * BOARD_ID_SIZE)) | (board.second >> (3 * BOARD_ID_SIZE));
        int t = 0;
        for (auto bit = top_pieces; bit != 0; bit &= (bit - 1)) {
            std::uint64_t p = bit & (-static_cast<std::int64_t>(bit));
            while ((p >> ((t + 1) * BOARD_ID_SIZE)) >= 1) ++t;
            board.second ^= p;
            auto result = check_status(board);

            // 決着していた場合は読まない
            if (!result) {
                for (auto bit2 = (~block_pieces) & (BOARD_MASK << (t * BOARD_ID_SIZE)); bit2 != 0; bit2 &= (bit2 - 1)) {
                    std::uint64_t p2 = bit2 & (-static_cast<std::int64_t>(bit2));
                    board.second ^= p2;
                    depth_search(board, depth, cnt, mp);
                    board.second ^= p2;
                }
            }

            board.second ^= p;
        }

        // 持ち駒を使う
        // 大きい駒から見る
        bool zero = false;
        for (int t = PIECE_TYPE_COUNT - 1; t >= 0; t--) {
            auto count = std::popcount(board.second & (BOARD_MASK << (t * BOARD_ID_SIZE)));
            //if (zero) break;
            if (count >= PIECE_EACH_COUNT) continue;
            zero |= (count == 0);
            for (auto bit = ((~block_pieces) & (BOARD_MASK << (t * BOARD_ID_SIZE))); bit != 0; bit &= (bit - 1)) {
                std::uint64_t p = bit & (-static_cast<std::int64_t>(bit));
                board.second ^= p;
                depth_search(board, depth, cnt, mp);
                board.second ^= p;
            }
        }
    }

    // 手番を戻す
    std::swap(board.first, board.second);
}

void simple_search(int max_depth, int& cnt) {
    Board board{};
    std::map<Board, bool> mp;
    depth_search(board, max_depth, cnt, mp);
}

