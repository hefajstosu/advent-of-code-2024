#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

const int DR[4] = {0, 1, 0, -1};  // kierunki ESWN
const int DC[4] = {1, 0, -1, 0};

using State = std::tuple<long long, int, int, int>;  // koszt, pozycja, kierunek

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<std::string> g;
    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) g.push_back(line);
    const int R = g.size(), C = g[0].size();
    int sr = 0, sc = 0, er = 0, ec = 0;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (g[r][c] == 'S') { sr = r; sc = c; }
            if (g[r][c] == 'E') { er = r; ec = c; }
        }

    auto idx = [&](int r, int c, int d) { return (r * C + c) * 4 + d; };

    // dijkstra, tez wstecz
    auto dijkstra = [&](const std::vector<State>& starts, bool backward) {
        std::vector<long long> dist(R * C * 4, LLONG_MAX);
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
        for (auto [d0, r, c, d] : starts) { dist[idx(r, c, d)] = d0; pq.push({d0, r, c, d}); }
        while (!pq.empty()) {
            auto [cost, r, c, d] = pq.top();
            pq.pop();
            if (cost > dist[idx(r, c, d)]) continue;
            int s = backward ? -1 : 1;
            std::vector<State> next = {
                {cost + 1, r + s * DR[d], c + s * DC[d], d},
                {cost + 1000, r, c, (d + 1) % 4},
                {cost + 1000, r, c, (d + 3) % 4},
            };
            for (auto [nc, nr, ncol, nd] : next) {
                if (g[nr][ncol] == '#') continue;
                if (nc < dist[idx(nr, ncol, nd)]) {
                    dist[idx(nr, ncol, nd)] = nc;
                    pq.push({nc, nr, ncol, nd});
                }
            }
        }
        return dist;
    };

    // najtanszy koszt
    std::vector<long long> fromS = dijkstra({{0, sr, sc, 0}}, false);
    long long best = LLONG_MAX;
    for (int d = 0; d < 4; ++d) best = std::min(best, fromS[idx(er, ec, d)]);

    // zliczamy dobre pola
    std::vector<State> ends;
    for (int d = 0; d < 4; ++d) ends.push_back({0, er, ec, d});
    std::vector<long long> toE = dijkstra(ends, true);
    long long tiles = 0;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            for (int d = 0; d < 4; ++d) {
                long long a = fromS[idx(r, c, d)], b = toE[idx(r, c, d)];
                if (a != LLONG_MAX && b != LLONG_MAX && a + b == best) { ++tiles; break; }
            }

    std::cout << "Czesc 1: " << best << '\n';
    std::cout << "Czesc 2: " << tiles << '\n';
}
