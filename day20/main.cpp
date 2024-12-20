#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    // prog oszczednosci
    int threshold = argc > 2 ? std::atoi(argv[2]) : 100;
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) grid.push_back(line);

    int h = grid.size(), w = grid[0].size();
    int sr = 0, sc = 0;
    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (grid[r][c] == 'S') sr = r, sc = c;

    // bfs od startu
    std::vector<std::vector<int>> dist(h, std::vector<int>(w, -1));
    std::queue<std::pair<int, int>> q;
    dist[sr][sc] = 0;
    q.push({sr, sc});
    const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    std::vector<std::pair<int, int>> track;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        track.push_back({r, c});
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            if (grid[nr][nc] == '#' || dist[nr][nc] != -1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }

    // liczymy skroty
    auto countCheats = [&](int maxCheat) {
        long long count = 0;
        for (auto [r, c] : track) {
            for (int ddr = -maxCheat; ddr <= maxCheat; ++ddr) {
                int rest = maxCheat - std::abs(ddr);
                for (int ddc = -rest; ddc <= rest; ++ddc) {
                    int nr = r + ddr, nc = c + ddc;
                    if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
                    if (dist[nr][nc] < 0) continue;
                    int d = std::abs(ddr) + std::abs(ddc);
                    if (dist[nr][nc] - dist[r][c] - d >= threshold) ++count;
                }
            }
        }
        return count;
    };

    std::cout << "Czesc 1: " << countCheats(2) << '\n';
    std::cout << "Czesc 2: " << countCheats(20) << '\n';
}
