// count.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "forward.h"
#include "cpp.h"

// 繰り返し二乗法
template<class T>
T my_pow(T x, T n) {
    T res = 1;
    while (n > 0) {
        if (n & 1) res = res * x;
        x = x * x;
        n >>= 1;
    }
    return res;
}

// ボードをゆっくり出力する
// tの単位はms
void debug_board(const Board& board, int t) {
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto [x, y] = board_id(id);
        std::cout << std::bitset<PIECE_TYPE_COUNT * 2>(board[id]);
        if (y == BOARD_SIZE - 1) std::cout << std::endl;
        else std::cout << " ";
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

// 2次元から1次元へ
int board_id(int x, int y) {
    assert(x >= 0 && x < BOARD_SIZE&& y >= 0 && y < BOARD_SIZE);
    return BOARD_SIZE * x + y;
}

// 1次元から2次元へ
std::pair<int, int> board_id(int id) {
    assert(id >= 0 && id < BOARD_ID_SIZE);
    return { id / BOARD_SIZE , id % BOARD_SIZE };
}

// boardのマスのフラッグから手番の人の駒だけ取り出す
// turn=0 => 先手
// turn=1 => 後手
int take_pieces(int square, int turn) {
    assert(turn == 0 || turn == 1);
    return (square >> (PIECE_TYPE_COUNT * turn)) & ((1 << PIECE_TYPE_COUNT) - 1);
}

// 手番の人の駒からboardのマスのフラッグに変換する
// turn=0 => 先手
// turn=1 => 後手
int convert_to_square(int square, int turn) {
    assert(turn == 0 || turn == 1);
    return square << PIECE_TYPE_COUNT * turn;
}

// 最も上にある1であるビットだけ取り出す
int bits_msb(int v)
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);
    v = v | (v >> 8);
    v = v | (v >> 16);
    return v ^ (v >> 1);
}

// 先手が勝ったらtrue, 負けたらfalse
// 勝ち負けが決まらない場合nullopt
std::optional<bool> check_status(const Board& board) {
    assert(std::all_of(board.begin(), board.end(), [](int b) { return (take_pieces(b, 0) & take_pieces(b, 1)) == 0; }));
    // 横
    for (int x = 0; x < BOARD_SIZE; x++) {
        bool f1 = true, f2 = true;
        for (int y = 0; y < BOARD_SIZE; y++) {
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0 || take_pieces(square, 0) < take_pieces(square, 1)) f1 = false;
            if (take_pieces(square, 1) == 0 || take_pieces(square, 1) < take_pieces(square, 0)) f2 = false;
        }
        if (f1) return true;
        if (f2) return false;
    }

    // 縦
    for (int y = 0; y < BOARD_SIZE; y++) {
        bool f1 = true, f2 = true;
        for (int x = 0; x < BOARD_SIZE; x++) {
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0 || take_pieces(square, 0) < take_pieces(square, 1)) f1 = false;
            if (take_pieces(square, 1) == 0 || take_pieces(square, 1) < take_pieces(square, 0)) f2 = false;
        }
        if (f1) return true;
        if (f2) return false;
    }

    // 左上から右下
    {
        bool f1 = true, f2 = true;
        for (int x = 0; x < BOARD_SIZE; x++) {
            int y = x;
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0 || take_pieces(square, 0) < take_pieces(square, 1)) f1 = false;
            if (take_pieces(square, 1) == 0 || take_pieces(square, 1) < take_pieces(square, 0)) f2 = false;
        }
        if (f1) return true;
        if (f2) return false;
    }

    // 右上から左下
    {
        bool f1 = true, f2 = true;
        for (int x = 0; x < BOARD_SIZE; x++) {
            int y = BOARD_SIZE - 1 - x;
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0 || take_pieces(square, 0) < take_pieces(square, 1)) f1 = false;
            if (take_pieces(square, 1) == 0 || take_pieces(square, 1) < take_pieces(square, 0)) f2 = false;
        }
        if (f1) return true;
        if (f2) return false;
    }

    return std::nullopt;
}

// 手番の人のボード上駒の個数を数える
// 特大、大、中、小
// turn=0 => 先手
// turn=1 => 後手
std::array<int, PIECE_TYPE_COUNT> count_board_pieces(const Board& board, int turn) {
    assert(turn == 0 || turn == 1);
    assert(std::all_of(board.begin(), board.end(), [](int b) { return (take_pieces(b, 0) & take_pieces(b, 1)) == 0; }));
    std::array<int, PIECE_TYPE_COUNT> count{};

    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto b = board[id] >> (PIECE_TYPE_COUNT * turn);
        for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
            if (b == 0) break;
            count[i] += b & 1;
            b >>= 1;
        }
    }

    assert(std::all_of(count.begin(), count.end(), [](int c) {return c <= PIECE_EACH_COUNT; }));
    return count;
}

// 時計回りに回転させたboard
Board rotate_board(const Board& board) {
    Board b{};
    // 各駒ごとに座標変換
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            b[board_id(y, BOARD_SIZE - 1 - x)] = board[board_id(x, y)];
        }
    }
    return b;
}

// 転置させたboard
Board transpose_board(const Board& board) {
    Board b{};
    // 各駒ごとに座標変換
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            b[board_id(y, x)] = board[board_id(x, y)];
        }
    }
    return b;
}

// プレイヤーを反転する
Board transpose_player_board(const Board& board) {
    Board b{};
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        assert(((board[id] >> PIECE_TYPE_COUNT) & ((board[id] & ((1 << PIECE_TYPE_COUNT) - 1)) << PIECE_TYPE_COUNT)) == 0);
        b[id] = (board[id] >> PIECE_TYPE_COUNT) | ((board[id] & ((1 << PIECE_TYPE_COUNT) - 1)) << PIECE_TYPE_COUNT);
    }
    return b;
}

// boardの比較演算子の実装
namespace std {
    template<>
    class less<Board> {
    public:
        constexpr bool operator()(const Board& x, const Board& y) const {
            for (int id = 0; id < BOARD_ID_SIZE; id++) {
                if (x[id] == y[id]) continue;
                return x[id] < y[id];
            }
            return false;
        }
    };
}

// メモ化有り深さ優先探索
// 対称な局面は同一視する
// 手番の人のboardが前にあるようにする
void depth_search(Board& board, int depth, int& cnt, std::map<Board, bool>& mp) {
    //debug_board(board, 1000);
    cnt += 1;
    if (depth == 0) return;

    {
        auto mn = board;
        auto tmp = board;
        // 転置
        for (int t = 0; t < 2; t++) {
            // 回転
            for (int i = 0; i < 4; i++) {
                mn = min(mn, tmp);
                tmp = rotate_board(tmp);
            }

            tmp = transpose_board(tmp);
        }

        if (mp.contains(mn)) return;
        mp[mn] = true;
    }

    // 終了局面かどうかを確認する
    auto result = check_status(board);
    if (result) return;

    // 今手番の人がboardのbitの後ろ側になる
    board = transpose_player_board(board);
    depth -= 1;


    // 盤上で駒を動かす
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto p1 = bits_msb(take_pieces(board[id], 1));
        auto p2 = take_pieces(board[id], 0);
        if (p1 == 0 || p1 < p2) continue;

        board[id] ^= convert_to_square(p1, 1);
        auto result = check_status(board);

        // 決着していた場合は読まない
        if (!result) {
            for (int id2 = 0; id2 < BOARD_ID_SIZE; id2++) {
                auto np = take_pieces(board[id2], 0) | take_pieces(board[id2], 1);
                if (id == id2 || np >= p1) continue;

                board[id2] ^= convert_to_square(p1, 1);
                depth_search(board, depth, cnt, mp);
                board[id2] ^= convert_to_square(p1, 1);
            }
        }

        board[id] ^= convert_to_square(p1, 1);
    }

    // 持ち駒を使う
    auto count = count_board_pieces(board, 1);
    for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
        if (count[i] == PIECE_EACH_COUNT) continue;

        const int piece = 1 << i;
        for (int id = 0; id < BOARD_ID_SIZE; id++) {
            auto p = take_pieces(board[id], 0) | take_pieces(board[id], 1);
            if (p >= piece) continue;

            board[id] ^= convert_to_square(piece, 1);
            depth_search(board, depth, cnt, mp);
            board[id] ^= convert_to_square(piece, 1);
        }
    }

    // 先後を戻す
    board = transpose_player_board(board);
    depth += 1;
}

void simple_search(int max_depth, int& cnt) {
    Board board{};
    std::map<Board, bool> mp;
    depth_search(board, max_depth, cnt, mp);
}

