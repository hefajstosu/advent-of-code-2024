#include <cstdio>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

int N;  // rozmiar siatki
std::vector<std::pair<int, int>> bytes;

// bfs po siatce
int shortest(int k) {
    std::vector<char> wall(N * N, 0);
    for (int i = 0; i < k; ++i) wall[bytes[i].second * N + bytes[i].first] = 1;
    std::vector<int> dist(N * N, -1);
    std::queue<int> q;
    dist[0] = 0;
    q.push(0);
    const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        int x = cur % N, y = cur / N;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
            int ni = ny * N + nx;
            if (wall[ni] || dist[ni] >= 0) continue;
            dist[ni] = dist[cur] + 1;
            q.push(ni);
        }
    }
    return dist[N * N - 1];
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::string line;
    int maxCoord = 0;
    while (std::getline(in, line)) {
        int x, y;
        if (std::sscanf(line.c_str(), "%d,%d", &x, &y) == 2) {
            bytes.push_back({x, y});
            maxCoord = std::max(maxCoord, std::max(x, y));
        }
    }

    // zgadujemy rozmiar
    N = maxCoord <= 6 ? 7 : 71;
    int first = maxCoord <= 6 ? 12 : 1024;

    // binsearch blokady
    int lo = 0, hi = bytes.size();
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (shortest(mid + 1) < 0) hi = mid;
        else lo = mid + 1;
    }
    std::string blocker = lo < (int)bytes.size()
        ? std::to_string(bytes[lo].first) + "," + std::to_string(bytes[lo].second)
        : "brak";

    std::cout << "Czesc 1: " << shortest(first) << '\n';
    std::cout << "Czesc 2: " << blocker << '\n';
}
