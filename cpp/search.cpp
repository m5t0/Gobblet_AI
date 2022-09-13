// count.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "forward.h"
#include "cpp.h"


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
std::uint32_t take_pieces(std::uint32_t square, int turn) {
    assert(turn == 0 || turn == 1);
    return (square >> (PIECE_TYPE_COUNT * turn)) & ((1 << PIECE_TYPE_COUNT) - 1);
}

// 手番の人の駒からboardのマスのフラッグに変換する
// turn=0 => 先手
// turn=1 => 後手
std::uint32_t convert_to_square(std::uint32_t square, int turn) {
    assert(turn == 0 || turn == 1);
    return square << PIECE_TYPE_COUNT * turn;
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

    assert(std::all_of(count.begin(), count.end(), [](int cnt) {return cnt <= PIECE_EACH_COUNT; }));
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

// Boardの比較演算子の実装
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
void depth_search(Board& board, long long depth, long long& cnt, long long& cnt2, std::map<Board, bool>& mp) {
    //debug_board(board, 1000);
    cnt += 1;

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

        if (mp.contains(mn)) {
            cnt2 += 1;
            return;
        }
        mp[mn] = true;
    }

    if (depth == 0) return;

    // 終了局面かどうかを確認する
    auto result = check_status(board);
    if (result) return;

    // 今手番の人がboardのbitの桁が大きい側になる
    board = transpose_player_board(board);
    depth -= 1;


    // 盤上で駒を動かす
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto p1 = std::bit_floor(take_pieces(board[id], 1));
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
                depth_search(board, depth, cnt, cnt2, mp);
                board[id2] ^= convert_to_square(p1, 1);
            }
        }

        board[id] ^= convert_to_square(p1, 1);
    }

    // 持ち駒を使う
    auto count = count_board_pieces(board, 1);
    for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
        if (count[i] == PIECE_EACH_COUNT) continue;

        const std::uint32_t piece = 1 << i;
        for (int id = 0; id < BOARD_ID_SIZE; id++) {
            auto p = take_pieces(board[id], 0) | take_pieces(board[id], 1);
            if (p >= piece) continue;

            board[id] ^= convert_to_square(piece, 1);
            depth_search(board, depth, cnt, cnt2, mp);
            board[id] ^= convert_to_square(piece, 1);
        }
    }

    // 先後を戻す
    board = transpose_player_board(board);
    depth += 1;
}

void simple_search(long long max_depth, long long& cnt, long long& cnt2, std::map<Board, bool>& mp) {
    Board board{};
    depth_search(board, max_depth, cnt, cnt2, mp);
}

struct Flag {
    // それぞれの駒が残り使える個数
    std::array<int, PIECE_TYPE_COUNT * 2> cnt;
    // 行判定, x = y斜め判定, x + y = BOARD_SIZE - 1斜め判定
    std::array<bool, 2> row, diag1, diag2;
    // 列判定
    std::array<std::array<bool, BOARD_SIZE>, 2> column;
    Flag(const std::array<int, PIECE_TYPE_COUNT * 2>& c) :cnt(c), row({ true, true }), diag1({ true,true }), diag2({ true, true }), column() {
        std::fill(column[0].begin(), column[0].end(), true);
        std::fill(column[1].begin(), column[1].end(), true);
    }

    Flag(const Flag& f) :cnt(f.cnt), row(f.row), diag1(f.diag1), diag2(f.diag2), column(f.column) {}
};

// 組み合わせの構造体
struct Comb {
    std::vector<long long> fac;
    Comb(size_t n) :fac(n + 1) {
        fac[0] = 1;
        for (size_t i = 0; i < n; i++) fac[i + 1] = fac[i] * (i + 1);
    }

    long long c(int n, int m) const {
        assert(n >= 0 && m >= 0);
        if (n < m) return 0;
        return (fac[n] / fac[m]) / fac[n - m];
    }
};

// 遅く、より速いアルゴリズムを思いついたので、ボツ
// 一番上にある駒だけ調べて最後にそれ以外の駒を数える
// board id, 先手がどの種類まで使っているか, 後手がどの種類まで使っているか, Flag, 使った駒の個数, 組み合わせの構造体
// 1列も揃っていない局面数を返す
boost::multiprecision::tom_int count_position(int id, int type, int type2, Flag f, std::array<int, 2 * PIECE_TYPE_COUNT>& used_cnt, const Comb& comb) {
    if (id != 0 && id % BOARD_SIZE == 0) {
        if (f.row[0] || f.row[1]) return 0;
        f.row[0] = f.row[1] = true;
    }
    if ((id-1) / BOARD_SIZE == BOARD_SIZE - 1 && (f.column[0][id - 1] || f.column[1][id - 1])) return 0;
    if (id == BOARD_ID_SIZE) {
        if (f.diag1[0] || f.diag2[0] || f.diag1[1] || f.diag2[1]) return 0;

        std::array<int, PIECE_TYPE_COUNT> sum{};
        for (int t = PIECE_TYPE_COUNT - 1; t >= 1; t--) {
            sum[t - 1] = sum[t] + used_cnt[t] + used_cnt[PIECE_TYPE_COUNT + t];
        }

        long long res = 1;
        for (int p = 0; p < 2; p++) {
            auto s = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT * p, used_cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
            for (int i = PIECE_TYPE_COUNT * p; i < PIECE_TYPE_COUNT * (p + 1); i++) {
                if (used_cnt[i] == 0) continue;
                res *= comb.c(s, used_cnt[i]);
                s -= used_cnt[i];
            }
        }

        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            long long tmp = 1;
            for (int i = 0; i <= std::min(sum[t], f.cnt[t]); i++) {
                for (int j = 0; j <= std::min(sum[t] - i, f.cnt[t + PIECE_TYPE_COUNT]); j++) {
                    if (i == 0 && j == 0) continue;
                    tmp += comb.c(sum[t], i) * comb.c(sum[t] - i, j);
                }
            }

            res *= tmp;
        }
        return res;
    }

    Flag b{ f };
    boost::multiprecision::tom_int res = 0;
    auto id2 = board_id(id);
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position(id + 1, type, type2, f, used_cnt, comb);

        f = b;
    }

    for (int t = 0; t < PIECE_TYPE_COUNT * 2; t++) {
        if (f.cnt[t] == 0 || (t < PIECE_TYPE_COUNT && t < type) || (t >= PIECE_TYPE_COUNT && (t - PIECE_TYPE_COUNT) < type2)) continue;
        auto i = (t < PIECE_TYPE_COUNT ? 1 : 0);
        auto nt1 = type, nt2 = type2;
        if (t < PIECE_TYPE_COUNT) nt1 = t;
        else nt2 = t - PIECE_TYPE_COUNT;

        f.cnt[t] -= 1;
        f.row[i] = false;
        f.column[i][id2.second] = false;
        if (id2.first == id2.second) f.diag1[i] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[i] = false;
        used_cnt[t] += 1;
        res += count_position(id + 1, nt1, nt2, f, used_cnt, comb);
        f = b;
        used_cnt[t] -= 1;
    }

    if (id < 10) std::cout << id << " " << res << std::endl;
    return res;
}

boost::multiprecision::tom_int count_position_wrapper() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt;
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);
    Flag f(cnt);
    std::array<int, 2 * PIECE_TYPE_COUNT> used_cnt{};
    Comb comb(BOARD_ID_SIZE + 3);
    return count_position(0, 0, 0, f, used_cnt, comb);
}

struct Flag2 {
    // それぞれの駒が使える個数
    std::array<int, PIECE_TYPE_COUNT * 2> cnt;
    // 先手と後手の残り使える駒の個数 先手と後手がそれぞれ使うことができる駒の数
    std::array<int, 2> mod_cnt, sum;
    // 行判定, x = y斜め判定, x + y = BOARD_SIZE - 1斜め判定
    std::array<bool, 2> row, diag1, diag2;
    // 列判定
    std::array<std::array<bool, BOARD_SIZE>, 2> column;
    Flag2(const std::array<int, PIECE_TYPE_COUNT * 2>& c) :cnt(c), mod_cnt(), row({ true, true }), diag1({ true,true }), diag2({ true, true }), column() {
        std::fill(column[0].begin(), column[0].end(), true);
        std::fill(column[1].begin(), column[1].end(), true);

        for (int p = 0; p < 2; p++) {
            mod_cnt[p] = sum[p] = std::accumulate(cnt.begin() + PIECE_TYPE_COUNT * p, cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
        }
    }

    Flag2(const Flag2& f) :cnt(f.cnt), mod_cnt(f.mod_cnt), sum(f.sum), row(f.row), diag1(f.diag1), diag2(f.diag2), column(f.column) {}
};

// 先手と後手の駒が少なくとも一つ存在するマスの位置と、盤で一番上(鉛直上向き)に置いてある先手と後手の駒の個数を固定したとき、その条件を満たす局面の数を返す関数
boost::multiprecision::tom_int cnt_dfs(int type, Flag2& f, std::array<int, 2 * PIECE_TYPE_COUNT>& used_cnt, const Comb& comb) {
    if (type == PIECE_TYPE_COUNT) {
        if (std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0) != f.sum[0] - f.mod_cnt[0] ||
            std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0) != f.sum[1] - f.mod_cnt[1]) {
            return 0;
        }

        std::array<int, PIECE_TYPE_COUNT> sum{};
        for (int t = PIECE_TYPE_COUNT - 1; t >= 1; t--) {
            sum[t - 1] = sum[t] + used_cnt[t] + used_cnt[t + PIECE_TYPE_COUNT];
        }

        boost::multiprecision::tom_int res = 1;
        for (int p = 0; p < 2; p++) {
            auto s = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT * p, used_cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
            for (int t = PIECE_TYPE_COUNT * p; t < PIECE_TYPE_COUNT * (p + 1); t++) {
                if (used_cnt[t] == 0) continue;
                res *= comb.c(s, used_cnt[t]);
                s -= used_cnt[t];
            }
        }

        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            long long tmp = 1;
            for (int i = 0; i <= std::min(sum[t], f.cnt[t] - used_cnt[t]); i++) {
                for (int j = 0; j <= std::min(sum[t] - i, f.cnt[t + PIECE_TYPE_COUNT] - used_cnt[t + PIECE_TYPE_COUNT]); j++) {
                    if (i == 0 && j == 0) continue;
                    tmp += comb.c(sum[t], i) * comb.c(sum[t] - i, j);
                }
            }

            res *= tmp;
        }

        return res;
    }

    auto s1 = std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0);
    auto s2 = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0);
    boost::multiprecision::tom_int res = 0;
    for (int i = 0; i <= std::min(f.cnt[type], f.sum[0] - f.mod_cnt[0] - s1); i++) {
        used_cnt[type] += i;
        for (int j = 0; j <= std::min(f.cnt[type + PIECE_TYPE_COUNT], f.sum[1] - f.mod_cnt[1] - s2); j++) {
            used_cnt[type + PIECE_TYPE_COUNT] += j;
            res += cnt_dfs(type + 1, f, used_cnt, comb);
            used_cnt[type + PIECE_TYPE_COUNT] -= j;
        }
        used_cnt[type] -= i;
    }

    return res;
}

// 1列も揃っていない局面数を調べる、より計算量が少なそうな解法
boost::multiprecision::tom_int count_position2(int id, Flag2 f, const Comb& comb, std::array<std::array<boost::multiprecision::tom_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& memo) {
    if (id != 0 && id % BOARD_SIZE == 0) {
        if (f.row[0] || f.row[1]) return 0;
        f.row[0] = f.row[1] = true;
    }
    if (id == BOARD_ID_SIZE) {
        if (std::any_of(f.column[0].begin(), f.column[0].end(), [](bool b) {return b; })
            || std::any_of(f.column[1].begin(), f.column[1].end(), [](bool b) {return b; })
            || f.diag1[0] || f.diag2[0] || f.diag1[1] || f.diag2[1]) return 0;

        auto used_cnt = std::array<int, 2 * PIECE_TYPE_COUNT>();
        if (memo[f.mod_cnt[0]][f.mod_cnt[1]] != -1) return memo[f.mod_cnt[0]][f.mod_cnt[1]];
        return memo[f.mod_cnt[0]][f.mod_cnt[1]] = cnt_dfs(0, f, used_cnt, comb);
    }

    Flag2 b{ f };
    boost::multiprecision::tom_int res = 0;
    auto id2 = board_id(id);
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position2(id + 1, f, comb, memo);

        f = b;
    }

    for (int p = 0; p < 2; p++) {
        if (f.mod_cnt[p] == 0) continue;
        f.mod_cnt[p] -= 1;
        f.row[p] = false;
        f.column[p][id2.second] = false;
        if (id2.first == id2.second) f.diag1[p] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        res += count_position2(id + 1, f, comb, memo);
        f = b;
    }

    //if (id < 5) std::cout << id << " " << res << std::endl;
    return res;
}

boost::multiprecision::tom_int count_position_wrapper2() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt{};
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);
    Flag2 f(cnt);
    Comb comb(BOARD_ID_SIZE + 3);
    std::array<std::array<boost::multiprecision::tom_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> memo;
    for (auto& m : memo) std::fill(m.begin(), m.end(), boost::multiprecision::tom_int(-1));
    return count_position2(0, f, comb, memo);
}

// 少なくとも1列は揃っている局面数
boost::multiprecision::tom_int count_position2_1(int id, bool ok, Flag2 f, const Comb& comb, std::array<std::array<boost::multiprecision::tom_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& memo) {
    if (id != 0 && id % BOARD_SIZE == 0) {
        ok = ok || f.row[0] || f.row[1];
        f.row[0] = f.row[1] = true;
    }
    if (id == BOARD_ID_SIZE) {
        if (!std::any_of(f.column[0].begin(), f.column[0].end(), [](bool b) {return b; })
            && !std::any_of(f.column[1].begin(), f.column[1].end(), [](bool b) {return b; })
            && !f.diag1[0] && !f.diag2[0] && !f.diag1[1] && !f.diag2[1] && !ok) return 0;

        auto used_cnt = std::array<int, 2 * PIECE_TYPE_COUNT>();
        if (memo[f.mod_cnt[0]][f.mod_cnt[1]] != -1) return memo[f.mod_cnt[0]][f.mod_cnt[1]];
        return memo[f.mod_cnt[0]][f.mod_cnt[1]] = cnt_dfs(0, f, used_cnt, comb);
    }

    Flag2 b{ f };
    boost::multiprecision::tom_int res = 0;
    auto id2 = board_id(id);
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position2_1(id + 1, ok, f, comb, memo);

        f = b;
    }

    for (int p = 0; p < 2; p++) {
        if (f.mod_cnt[p] == 0) continue;
        f.mod_cnt[p] -= 1;
        f.row[p] = false;
        f.column[p][id2.second] = false;
        if (id2.first == id2.second) f.diag1[p] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        res += count_position2_1(id + 1, ok, f, comb, memo);
        f = b;
    }

    //if (id < 5) std::cout << id << " " << res << std::endl;
    return res;
}

boost::multiprecision::tom_int count_position_wrapper2_1() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt{};
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);
    //std::fill(cnt.begin(), cnt.begin()+PIECE_TYPE_COUNT, 1);
    Flag2 f(cnt);
    Comb comb(BOARD_ID_SIZE + 3);
    std::array<std::array<boost::multiprecision::tom_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> memo;
    for (auto& m : memo) std::fill(m.begin(), m.end(), boost::multiprecision::tom_int(-1));
    return count_position2_1(0, false, f, comb, memo);
}

// 物理的にあり得る局面数を計算する
// 答えが5'972'215'100'065'114'586'401
boost::multiprecision::tom_int count_position_all() {
    Comb comb(BOARD_ID_SIZE + 3);
    boost::multiprecision::tom_int cnt = 1;
    for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
        boost::multiprecision::tom_int tmp = 1;
        for (int i0 = 0; i0 <= PIECE_EACH_COUNT; i0++) {
            for (int i1 = 0; i1 <= PIECE_EACH_COUNT; i1++) {
                if (i0 == 0 && i1 == 0) continue;
                tmp += boost::multiprecision::tom_int(comb.c(BOARD_ID_SIZE, i0)) * boost::multiprecision::tom_int(comb.c(BOARD_ID_SIZE - i0, i1));
            }
        }
        std::cout << cnt << " " << tmp << std::endl;
        cnt *= tmp;
    }

    return cnt;
}
