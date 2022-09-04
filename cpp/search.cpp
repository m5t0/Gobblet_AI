// count.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "forward.h"
#include "cpp.h"


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
std::optional<bool> check_status(Board& board) {
    assert(std::all_of(board.begin(), board.end(), [](int b) { return (take_pieces(b, 0) & take_pieces(b, 1)) == 0; }));
    // 横
    for (int x = 0; x < BOARD_SIZE; x++) {
        bool f1 = true, f2 = true;
        for (int y = 0; y < BOARD_SIZE; y++) {
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0) f1 = false;
            if (take_pieces(square, 1) == 0) f2 = false;
        }
        if (f1) return true;
        if (f2) return false;
    }

    // 縦
    for (int y = 0; y < BOARD_SIZE; y++) {
        bool f1 = true, f2 = true;
        for (int x = 0; x < BOARD_SIZE; x++) {
            auto square = board[board_id(x, y)];
            if (take_pieces(square, 0) == 0) f1 = false;
            if (take_pieces(square, 1) == 0) f2 = false;
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
            if (take_pieces(square, 0) == 0) f1 = false;
            if (take_pieces(square, 1) == 0) f2 = false;
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
            if (take_pieces(square, 0) == 0) f1 = false;
            if (take_pieces(square, 1) == 0) f2 = false;
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
std::array<int, PIECE_TYPE_COUNT> count_board_pieces(Board& board, int turn) {
    assert(turn == 0 || turn == 1);
    assert(std::all_of(board.begin(), board.end(), [](int b) { return (take_pieces(b, 0) & take_pieces(b, 1)) == 0; }));
    std::array<int, PIECE_TYPE_COUNT> count;
    std::fill(count.begin(), count.end(), 0);

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

// ゴブレットの駒の種類数や個数が変わると使えないことに注意
// 小、中、大、特大のある場所で管理をする
// それをさらに(小、中)と(大、特大)の組にする
// boardからpair<int64, int64>に変換
std::pair<long long, long long> board_to_pair(Board& board) {
    // 小、中、大、特大の順番
    // 同じ種類の中では先手、後手の順番にPIECE_EACH_COUNT個ずつ入っている
    std::array<int, PIECE_TYPE_COUNT> p, cnt;
    std::fill(p.begin(), p.end(), 0);
    std::fill(cnt.begin(), cnt.end(), 0);

    // 後手を先に処理する
    for (int turn = 1; turn >= 0; turn--) {
        // 盤上にある駒に対してidを追加
        // 大きい駒を先に処理する
        for (int t = PIECE_TYPE_COUNT - 1; t >= 0; t--) {
            for (int id = 0; id < BOARD_ID_SIZE; id++) {
                if (((take_pieces(board[id], turn) >> t) & 1) != 1) continue;
                p[t] *= (BOARD_ID_SIZE + 1);
                p[t] += id;
                cnt[t] += 1;
            }
        }

        assert(std::all_of(cnt.begin(), cnt.end(), [](int x) {return x <= PIECE_EACH_COUNT; }));

        // 持ち駒のidを追加
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            while (cnt[t] < PIECE_EACH_COUNT) {
                p[t] *= (BOARD_ID_SIZE + 1);
                p[t] += BOARD_ID_SIZE;
                cnt[t] += 1;
            }
        }

        std::fill(cnt.begin(), cnt.end(), 0);
    }

    return { (static_cast<long long>(p[0]) << 32) | p[1],(static_cast<long long>(p[2]) << 32) | p[3] };
}

// pair<int64, int64>からboardに変換
Board pair_to_board(std::pair<long long, long long>& p) {
    std::array<int, PIECE_TYPE_COUNT> a{
        static_cast<int>(p.first >> 32), static_cast<int>(p.first & ((1LL << 32) - 1)),
        static_cast<int>(p.second >> 32), static_cast<int>(p.second & ((1LL << 32) - 1)),
    };

    Board board;
    std::fill(board.begin(), board.end(), 0);

    for (int turn = 0; turn < 2; turn++) {
        for (int t = 0; t < PIECE_TYPE_COUNT; t++) {
            for (int c = 0; c < PIECE_EACH_COUNT; c++) {
                auto id = a[t] % (BOARD_ID_SIZE + 1);
                assert(id >= 0 && id <= BOARD_ID_SIZE);
                if (id != BOARD_ID_SIZE) board[id] |= convert_to_square(1 << t, turn);
                a[t] /= (BOARD_ID_SIZE + 1);
            }
        }
    }

    return board;
}

// hashの独自実装
class my_hash {
public:
    size_t operator()(const std::pair<long long, long long>& p) const {
        auto hash1 = std::hash<long long>{}(p.first);
        auto hash2 = std::hash<long long>{}(p.second);

        size_t seed = 0;
        seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};


// ボードをゆっくり出力する
// tの単位はms
void debug_board(Board& board, int t) {
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto [x, y] = board_id(id);
        std::cout << std::bitset<PIECE_TYPE_COUNT * 2>(board[id]);
        if (y == BOARD_SIZE - 1) std::cout << std::endl;
        else std::cout << " ";
    }
    std::cout << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}

// 盤をそのままqueueに突っ込む
// 幅優先探索、メモ化無し、　駒の個数はその場で数える
// 計算量:中, メモリ消費量:中
void simple_search1() {
    // 手番を一番最後に加える。
    // 先手:0, 後手:1
    // board, turn, depth
    std::queue<std::tuple<Board, int, int>> que;
    que.emplace();
    int d = 0, cnt = 0;
    auto start = std::chrono::system_clock::now();
    while (!que.empty()) {
        auto [board, turn, depth] = que.front();
        que.pop();

        if (depth > d) {
            auto now = std::chrono::system_clock::now();
            auto t = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            std::cout << t << "ms depth:" << depth << ", cnt:" << cnt << ", queue size:" << que.size() << std::endl;

            d = depth;
            cnt = 1;
        }
        else {
            cnt += 1;
        }

        //debug_board(board, 100);

        // 終了局面かどうかを確認する
        auto result = check_status(board);
        if (result) continue;

        // 盤上で駒を動かす
        for (int id = 0; id < BOARD_ID_SIZE; id++) {
            auto p1 = bits_msb(take_pieces(board[id], turn));
            auto p2 = take_pieces(board[id], turn ^ 1);
            if (p1 == 0 || p1 < p2) continue;

            board[id] ^= convert_to_square(p1, turn);
            auto result = check_status(board);

            // 決着していた場合は読まない
            if (!result) {
                for (int id2 = 0; id2 < BOARD_ID_SIZE; id2++) {
                    auto np1 = take_pieces(board[id2], turn);
                    auto np2 = take_pieces(board[id2], turn ^ 1);
                    if (id == id2 || np1 >= p1 || np2 >= p1) continue;

                    board[id2] ^= convert_to_square(p1, turn);
                    que.emplace(board, turn ^ 1, depth + 1);
                    board[id2] ^= convert_to_square(p1, turn);
                }
            }

            board[id] ^= convert_to_square(p1, turn);
        }

        // 持ち駒を使う
        auto count = count_board_pieces(board, turn);
        for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
            if (count[i] == PIECE_EACH_COUNT) continue;

            const int piece = 1 << i;
            for (int id = 0; id < BOARD_ID_SIZE; id++) {
                auto p1 = take_pieces(board[id], turn);
                auto p2 = take_pieces(board[id], turn ^ 1);
                if (p1 >= piece || p2 >= piece) continue;

                board[id] ^= convert_to_square(piece, turn);
                que.emplace(board, turn ^ 1, depth + 1);
                board[id] ^= convert_to_square(piece, turn);
            }
        }
    }
}

// メモ化なし深さ優先探索
void depth_search1(Board& board, int turn, int& depth, int& cnt) {
    cnt += 1;
    if (depth == 0) return;

    //debug_board(board,300);

    // 終了局面かどうかを確認する
    auto result = check_status(board);
    if (result) return;

    // 盤上で駒を動かす
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto p1 = bits_msb(take_pieces(board[id], turn));
        auto p2 = take_pieces(board[id], turn ^ 1);
        if (p1 == 0 || p1 < p2) continue;

        board[id] ^= convert_to_square(p1, turn);
        auto result = check_status(board);

        // 決着していた場合は読まない
        if (!result) {
            depth -= 1;
            for (int id2 = 0; id2 < BOARD_ID_SIZE; id2++) {
                auto np1 = take_pieces(board[id2], turn);
                auto np2 = take_pieces(board[id2], turn ^ 1);
                if (id == id2 || np1 >= p1 || np2 >= p1) continue;

                board[id2] ^= convert_to_square(p1, turn);
                depth_search1(board, turn ^ 1, depth, cnt);
                board[id2] ^= convert_to_square(p1, turn);
            }
            depth += 1;
        }

        board[id] ^= convert_to_square(p1, turn);
    }

    // 持ち駒を使う
    auto count = count_board_pieces(board, turn);
    for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
        if (count[i] == PIECE_EACH_COUNT) continue;

        const int piece = 1 << i;
        depth -= 1;
        for (int id = 0; id < BOARD_ID_SIZE; id++) {
            auto p1 = take_pieces(board[id], turn);
            auto p2 = take_pieces(board[id], turn ^ 1);
            if (p1 >= piece || p2 >= piece) continue;

            board[id] ^= convert_to_square(piece, turn);
            depth_search1(board, turn ^ 1, depth, cnt);
            board[id] ^= convert_to_square(piece, turn);
        }
        depth += 1;
    }
}

// 深さ優先探索、メモ化無し、boardを変化で保存する分メモリ消費量が少なく速い
// その代わり深さが固定される
void simple_search2(int max_depth, int& cnt) {
    Board board;
    std::fill(board.begin(), board.end(), 0);
    depth_search1(board, 0, max_depth, cnt);
}

// メモ化有り深さ優先探索
void depth_search2(Board& board, int turn, int& depth, int& cnt, std::unordered_map<std::pair<long long, long long>, bool, my_hash>& hash) {
    auto hp = board_to_pair(board);
    if (hash.find(hp) != hash.end()) return;
    hash[hp] = true;
    cnt += 1;
    if (depth == 0) return;

    //debug_board(board,300);

    // 終了局面かどうかを確認する
    auto result = check_status(board);
    if (result) return;

    // 盤上で駒を動かす
    for (int id = 0; id < BOARD_ID_SIZE; id++) {
        auto p1 = bits_msb(take_pieces(board[id], turn));
        auto p2 = take_pieces(board[id], turn ^ 1);
        if (p1 == 0 || p1 < p2) continue;

        board[id] ^= convert_to_square(p1, turn);
        auto result = check_status(board);

        // 決着していた場合は読まない
        if (!result) {
            depth -= 1;
            for (int id2 = 0; id2 < BOARD_ID_SIZE; id2++) {
                auto np1 = take_pieces(board[id2], turn);
                auto np2 = take_pieces(board[id2], turn ^ 1);
                if (id == id2 || np1 >= p1 || np2 >= p1) continue;

                board[id2] ^= convert_to_square(p1, turn);
                depth_search2(board, turn ^ 1, depth, cnt, hash);
                board[id2] ^= convert_to_square(p1, turn);
            }
            depth += 1;
        }

        board[id] ^= convert_to_square(p1, turn);
    }

    // 持ち駒を使う
    auto count = count_board_pieces(board, turn);
    for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
        if (count[i] == PIECE_EACH_COUNT) continue;

        const int piece = 1 << i;
        depth -= 1;
        for (int id = 0; id < BOARD_ID_SIZE; id++) {
            auto p1 = take_pieces(board[id], turn);
            auto p2 = take_pieces(board[id], turn ^ 1);
            if (p1 >= piece || p2 >= piece) continue;

            board[id] ^= convert_to_square(piece, turn);
            depth_search2(board, turn ^ 1, depth, cnt, hash);
            board[id] ^= convert_to_square(piece, turn);
        }
        depth += 1;
    }
}

// 深さ優先探索、unordered_mapでメモ化、色々なものを変化で保存する
// 同じ局面を探索しないようになり、queueの分のメモリは削減できる
// ただハッシュ計算の分1局面当たりの探索速度は遅くなる
void simple_search3(int max_depth, int& cnt) {
    Board board;
    std::unordered_map<std::pair<long long, long long>, bool, my_hash> hash;
    std::fill(board.begin(), board.end(), 0);
    depth_search2(board, 0, max_depth, cnt, hash);
}
