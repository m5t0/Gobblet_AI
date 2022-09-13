// count.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "forward.h"
#include "cpp.h"

using cpp_int = boost::multiprecision::cpp_int;

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

// 組み合わせの構造体
struct Comb {
    std::vector<cpp_int> fac;
    Comb(size_t n) :fac(n + 1) {
        fac[0] = 1;
        for (size_t i = 0; i < n; i++) fac[i + 1] = fac[i] * (i + 1);
    }

    cpp_int c(int n, int m) const {
        assert(n >= 0 && m >= 0);
        if (n < m) return 0;
        return (fac[n] / fac[m]) / fac[n - m];
    }
};

struct Flag {
    // それぞれの駒が使える個数
    std::array<int, PIECE_TYPE_COUNT * 2> cnt;
    // 先手と後手の残り使える駒の個数 先手と後手がそれぞれ使うことができる駒の数
    std::array<int, 2> mod_cnt, sum;
    // 行判定, x = y斜め判定, x + y = BOARD_SIZE - 1斜め判定
    std::array<bool, 2> row, diag1, diag2;
    // 列判定
    std::array<std::array<bool, BOARD_SIZE>, 2> column;
    Flag(const std::array<int, PIECE_TYPE_COUNT * 2>& c) :cnt(c), mod_cnt(), row({ true, true }), diag1({ true,true }), diag2({ true, true }), column() {
        std::fill(column[0].begin(), column[0].end(), true);
        std::fill(column[1].begin(), column[1].end(), true);

        for (int p = 0; p < 2; p++) {
            mod_cnt[p] = sum[p] = std::accumulate(cnt.begin() + PIECE_TYPE_COUNT * p, cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
        }
    }

    Flag(const Flag& f) :cnt(f.cnt), mod_cnt(f.mod_cnt), sum(f.sum), row(f.row), diag1(f.diag1), diag2(f.diag2), column(f.column) {}
};

// 答え保存用のmapの比較演算子
namespace std {
    template<>
    class less<std::array<int, 2 * PIECE_TYPE_COUNT>> {
    public:
        constexpr bool operator()(const std::array<int, 2 * PIECE_TYPE_COUNT>& x, const std::array<int, 2 * PIECE_TYPE_COUNT>& y) const {
            for (int id = 0; id < 2 * PIECE_TYPE_COUNT; id++) {
                if (x[id] == y[id]) continue;
                return x[id] < y[id];
            }
            return false;
        }
    };
}

// 先手と後手の駒が少なくとも一つ存在するマスの位置と、盤で一番上(鉛直上向き)に置いてある先手と後手の駒の個数を固定したとき、その条件を満たす局面の数を返す関数
cpp_int cnt_dfs(int type, Flag& f, std::array<int, 2 * PIECE_TYPE_COUNT>& used_cnt, const Comb& comb) {
    // それぞれの種類の駒に対して全探索が終わった
    if (type == PIECE_TYPE_COUNT) {
        if (std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0) != f.sum[0] - f.mod_cnt[0] ||
            std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0) != f.sum[1] - f.mod_cnt[1]) {
            return 0;
        }

        // それぞれの種類の駒に対して、使える駒の個数の最大値を求める
        std::array<int, PIECE_TYPE_COUNT> max_cnt{};
        for (int t = PIECE_TYPE_COUNT - 1; t >= 1; t--) {
            max_cnt[t - 1] = max_cnt[t] + used_cnt[t] + used_cnt[t + PIECE_TYPE_COUNT];
        }

        cpp_int res = 1;

        // 一番上に置く駒の置き方の組み合わせを計算する
        for (int p = 0; p < 2; p++) {
            auto s = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT * p, used_cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
            for (int t = PIECE_TYPE_COUNT * p; t < PIECE_TYPE_COUNT * (p + 1); t++) {
                if (used_cnt[t] == 0) continue;
                res *= comb.c(s, used_cnt[t]);
                s -= used_cnt[t];
            }
        }

        // 上に駒がある駒の置き方の組み合わせを計算する
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            cpp_int tmp = 1;
            for (int i = 0; i <= std::min(max_cnt[t], f.cnt[t] - used_cnt[t]); i++) {
                for (int j = 0; j <= std::min(max_cnt[t] - i, f.cnt[t + PIECE_TYPE_COUNT] - used_cnt[t + PIECE_TYPE_COUNT]); j++) {
                    if (i == 0 && j == 0) continue;
                    tmp += comb.c(max_cnt[t], i) * comb.c(max_cnt[t] - i, j);
                }
            }

            res *= tmp;
        }

        return res;
    }

    // 盤の一番上にすでに置いている駒の個数を先手と後手でそれぞれ計算する
    auto s1 = std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0);
    auto s2 = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0);

    cpp_int res = 0;
    // 種類typeの駒を何個使うのかを全探索する
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

// 1列も揃っていない局面数を調べる関数
// それぞれのマスに対して一番上(鉛直上向き)にある駒が先手の駒か後手の駒か、若しくは駒が置かれていないかを全探索し、
// 一番上にある先手の駒の個数と後手の駒の個数を数える
cpp_int count_position(int id, Flag f, const Comb& comb, std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& memo) {
    // 行が揃っていたら枝切り
    if (id != 0 && id % BOARD_SIZE == 0) {
        if (f.row[0] || f.row[1]) return 0;
        f.row[0] = f.row[1] = true;
    }
    // 列が揃っていたら枝切り
    if ((((id - 1) / BOARD_SIZE) == (BOARD_SIZE - 1)) && (f.column[0][(id - 1) % BOARD_SIZE] || f.column[1][(id - 1) % BOARD_SIZE])) return 0;
    // x+y=BOARD_SIZE方向の斜めが揃っていたら枝切り
    if ((id - 1) == BOARD_SIZE * (BOARD_SIZE - 1) && (f.diag2[0] || f.diag2[1])) return 0;

    // 盤の端まで来たとき
    if (id == BOARD_ID_SIZE) {
        // x=y方向の斜めが揃っていないか確認
        if (f.diag1[0] || f.diag1[1]) return 0;

        // 先手と後手の使っていない駒の個数でメモ化
        if (memo[f.mod_cnt[0]][f.mod_cnt[1]] != -1) return memo[f.mod_cnt[0]][f.mod_cnt[1]];

        auto used_cnt = std::array<int, 2 * PIECE_TYPE_COUNT>();
        return memo[f.mod_cnt[0]][f.mod_cnt[1]] = cnt_dfs(0, f, used_cnt, comb);
    }

    Flag b{ f };
    cpp_int res = 0;
    auto id2 = board_id(id);

    // 先手の駒も後手の駒もマスに置かない
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position(id + 1, f, comb, memo);

        f = b;
    }

    // 先手の駒or後手の駒を盤の一番上に置く
    for (int p = 0; p < 2; p++) {
        if (f.mod_cnt[p] == 0) continue;
        f.mod_cnt[p] -= 1;
        f.row[p] = false;
        f.column[p][id2.second] = false;
        if (id2.first == id2.second) f.diag1[p] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        res += count_position(id + 1, f, comb, memo);
        f = b;
    }

    //if (id < 5) std::cout << id << " " << res << std::endl;
    return res;
}

cpp_int count_position_wrapper() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt{};
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);

    Flag f(cnt);
    Comb comb(BOARD_ID_SIZE + 3);
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> memo;
    for (auto& m : memo) std::fill(m.begin(), m.end(), cpp_int(-1));

    return count_position(0, f, comb, memo);
}

// 少なくとも1列は揃っている局面数
cpp_int count_position2(int id, bool ok, Flag f, const Comb& comb, std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& memo) {
    // 少なくとも一つの行が揃っているかどうかをokで管理する
    if (id != 0 && id % BOARD_SIZE == 0) {
        ok = ok || f.row[0] || f.row[1];
        f.row[0] = f.row[1] = true;
    }
    if (id == BOARD_ID_SIZE) {
        // 行や斜めの少なくとも一つが揃っているかどうかを確認する
        if (!std::any_of(f.column[0].begin(), f.column[0].end(), [](bool b) {return b; })
            && !std::any_of(f.column[1].begin(), f.column[1].end(), [](bool b) {return b; })
            && !f.diag1[0] && !f.diag1[1] && !f.diag2[0] && !f.diag2[1] && !ok) return 0;

        auto used_cnt = std::array<int, 2 * PIECE_TYPE_COUNT>();
        if (memo[f.mod_cnt[0]][f.mod_cnt[1]] != -1) return memo[f.mod_cnt[0]][f.mod_cnt[1]];
        return memo[f.mod_cnt[0]][f.mod_cnt[1]] = cnt_dfs(0, f, used_cnt, comb);
    }

    Flag b{ f };
    cpp_int res = 0;
    auto id2 = board_id(id);

    // 先手の駒も後手の駒もマスに置かない
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position2(id + 1, ok, f, comb, memo);

        f = b;
    }

    // 先手の駒or後手の駒を盤の一番上に置く
    for (int p = 0; p < 2; p++) {
        if (f.mod_cnt[p] == 0) continue;
        f.mod_cnt[p] -= 1;
        f.row[p] = false;
        f.column[p][id2.second] = false;
        if (id2.first == id2.second) f.diag1[p] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        res += count_position2(id + 1, ok, f, comb, memo);
        f = b;
    }

    //if (id < 5) std::cout << id << " " << res << std::endl;
    return res;
}

cpp_int count_position_wrapper2() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt{};
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);

    Flag f(cnt);
    Comb comb(BOARD_ID_SIZE + 3);
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> memo;
    for (auto& m : memo) std::fill(m.begin(), m.end(), cpp_int(-1));

    return count_position2(0, false, f, comb, memo);
}

// 先手と後手の駒が少なくとも一つ存在するマスの位置と、盤で一番上(鉛直上向き)に置いてある先手と後手の駒の個数を固定したとき、その条件を満たす局面の数を返す関数
// mapで駒の個数ごとに保存しておく
cpp_int cnt_dfs2(int type, Flag& f, std::array<int, 2 * PIECE_TYPE_COUNT>& used_cnt, const Comb& comb,
    std::map<std::array<int, 2 * PIECE_TYPE_COUNT>, cpp_int>& mp) {
    // それぞれの種類の駒に対して全探索が終わった
    if (type == PIECE_TYPE_COUNT) {
        if (std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0) != f.sum[0] - f.mod_cnt[0] ||
            std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0) != f.sum[1] - f.mod_cnt[1]) {
            return 0;
        }

        // それぞれの種類の駒に対して、使える駒の個数の最大値を求める
        std::array<int, PIECE_TYPE_COUNT> max_cnt{};
        for (int t = PIECE_TYPE_COUNT - 1; t >= 1; t--) {
            max_cnt[t - 1] = max_cnt[t] + used_cnt[t] + used_cnt[t + PIECE_TYPE_COUNT];
        }

        cpp_int res = 1;

        // 一番上に置く駒の置き方の組み合わせを計算する
        for (int p = 0; p < 2; p++) {
            auto s = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT * p, used_cnt.begin() + PIECE_TYPE_COUNT * (p + 1), 0);
            for (int t = PIECE_TYPE_COUNT * p; t < PIECE_TYPE_COUNT * (p + 1); t++) {
                if (used_cnt[t] == 0) continue;
                res *= comb.c(s, used_cnt[t]);
                s -= used_cnt[t];
            }
        }

        // 上に駒がある駒の置き方の組み合わせを計算する
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            cpp_int tmp = 1;
            for (int i = 0; i <= std::min(max_cnt[t], f.cnt[t] - used_cnt[t]); i++) {
                for (int j = 0; j <= std::min(max_cnt[t] - i, f.cnt[t + PIECE_TYPE_COUNT] - used_cnt[t + PIECE_TYPE_COUNT]); j++) {
                    if (i == 0 && j == 0) continue;
                    tmp += comb.c(max_cnt[t], i) * comb.c(max_cnt[t] - i, j);
                }
            }

            res *= tmp;
        }

        mp[used_cnt] += res;

        return res;
    }

    // 盤の一番上にすでに置いている駒の個数を先手と後手でそれぞれ計算する
    auto s1 = std::accumulate(used_cnt.begin(), used_cnt.begin() + PIECE_TYPE_COUNT, 0);
    auto s2 = std::accumulate(used_cnt.begin() + PIECE_TYPE_COUNT, used_cnt.begin() + PIECE_TYPE_COUNT * 2, 0);

    cpp_int res = 0;
    // 種類typeの駒を何個使うのかを全探索する
    for (int i = 0; i <= std::min(f.cnt[type], f.sum[0] - f.mod_cnt[0] - s1); i++) {
        used_cnt[type] += i;
        for (int j = 0; j <= std::min(f.cnt[type + PIECE_TYPE_COUNT], f.sum[1] - f.mod_cnt[1] - s2); j++) {
            used_cnt[type + PIECE_TYPE_COUNT] += j;
            res += cnt_dfs2(type + 1, f, used_cnt, comb, mp);
            used_cnt[type + PIECE_TYPE_COUNT] -= j;
        }
        used_cnt[type] -= i;
    }

    return res;
}

// 1列も揃っていない局面数を調べる関数
// それぞれのマスに対して一番上(鉛直上向き)にある駒が先手の駒か後手の駒か、若しくは駒が置かれていないかを全探索し、
// 一番上にある先手の駒の個数と後手の駒の個数を数える
cpp_int count_position3(int id, Flag f, const Comb& comb,
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& memo,
    std::array<std::array<std::map<std::array<int, 2 * PIECE_TYPE_COUNT>, cpp_int>, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& cnt,
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>& mp_cnt) {
    // 行が揃っていたら枝切り
    if (id != 0 && id % BOARD_SIZE == 0) {
        if (f.row[0] || f.row[1]) return 0;
        f.row[0] = f.row[1] = true;
    }
    // 列が揃っていたら枝切り
    if ((((id - 1) / BOARD_SIZE) == (BOARD_SIZE - 1)) && (f.column[0][(id - 1) % BOARD_SIZE] || f.column[1][(id - 1) % BOARD_SIZE])) return 0;
    // x+y=BOARD_SIZE方向の斜めが揃っていたら枝切り
    if ((id - 1) == BOARD_SIZE * (BOARD_SIZE - 1) && (f.diag2[0] || f.diag2[1])) return 0;

    // 盤の端まで来たとき
    if (id == BOARD_ID_SIZE) {
        // x=y方向の斜めが揃っていないか確認
        if (f.diag1[0] || f.diag1[1]) return 0;

        mp_cnt[f.mod_cnt[0]][f.mod_cnt[1]] += 1;

        // 先手と後手の使っていない駒の個数でメモ化
        if (memo[f.mod_cnt[0]][f.mod_cnt[1]] != -1) return memo[f.mod_cnt[0]][f.mod_cnt[1]];

        auto used_cnt = std::array<int, 2 * PIECE_TYPE_COUNT>();
        std::map<std::array<int, 2 * PIECE_TYPE_COUNT>, cpp_int> mp;
        memo[f.mod_cnt[0]][f.mod_cnt[1]] = cnt_dfs2(0, f, used_cnt, comb, mp);
        cnt[f.mod_cnt[0]][f.mod_cnt[1]] = std::move(mp);

        return memo[f.mod_cnt[0]][f.mod_cnt[1]];
    }

    Flag b{ f };
    cpp_int res = 0;
    auto id2 = board_id(id);

    // 先手の駒も後手の駒もマスに置かない
    {
        for (int p = 0; p < 2; p++) {
            f.row[p] = false;
            f.column[p][id2.second] = false;
            if (id2.first == id2.second) f.diag1[p] = false;
            if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        }
        res += count_position3(id + 1, f, comb, memo, cnt, mp_cnt);

        f = b;
    }

    // 先手の駒or後手の駒を盤の一番上に置く
    for (int p = 0; p < 2; p++) {
        if (f.mod_cnt[p] == 0) continue;
        f.mod_cnt[p] -= 1;
        f.row[p] = false;
        f.column[p][id2.second] = false;
        if (id2.first == id2.second) f.diag1[p] = false;
        if (id2.first + id2.second == BOARD_SIZE - 1) f.diag2[p] = false;
        res += count_position3(id + 1, f, comb, memo, cnt, mp_cnt);
        f = b;
    }

    //if (id < 5) std::cout << id << " " << res << std::endl;
    return res;
}

std::pair<
    std::array<std::array<std::map<std::array<int, 2 * PIECE_TYPE_COUNT>, cpp_int>, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>,
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1>
> count_position_wrapper3() {
    std::array<int, PIECE_TYPE_COUNT * 2> cnt{};
    std::fill(cnt.begin(), cnt.end(), PIECE_EACH_COUNT);

    Flag f(cnt);
    Comb comb(BOARD_ID_SIZE + 3);
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> memo;
    for (auto& m : memo) std::fill(m.begin(), m.end(), cpp_int(-1));
    std::array<std::array<std::map<std::array<int, 2 * PIECE_TYPE_COUNT>, cpp_int>, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> cnt2{};
    std::array<std::array<cpp_int, PIECE_PLAYER_COUNT + 1>, PIECE_PLAYER_COUNT + 1> mp_cnt{};

    count_position3(0, f, comb, memo, cnt2, mp_cnt);

    return { cnt2, mp_cnt };
}

// 物理的にあり得る局面数を計算する
cpp_int count_position_all() {
    Comb comb(BOARD_ID_SIZE + 3);
    cpp_int cnt = 1;
    for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
        cpp_int tmp = 1;
        for (int i0 = 0; i0 <= PIECE_EACH_COUNT; i0++) {
            for (int i1 = 0; i1 <= PIECE_EACH_COUNT; i1++) {
                if (i0 == 0 && i1 == 0) continue;
                tmp += cpp_int(comb.c(BOARD_ID_SIZE, i0)) * cpp_int(comb.c(BOARD_ID_SIZE - i0, i1));
            }
        }
        //std::cout << cnt << " " << tmp << std::endl;
        cnt *= tmp;
    }

    return cnt;
}
