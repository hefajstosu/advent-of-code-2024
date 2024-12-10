#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> grid;
int h, w;
const int DR[] = {-1, 1, 0, 0};
const int DC[] = {0, 0, -1, 1};

// dfs po szlakach
long long walk(int r, int c, std::set<std::pair<int, int>>& peaks) {
    if (grid[r][c] == '9') {
        peaks.insert({r, c});
        return 1;
    }
    long long paths = 0;
    for (int d = 0; d < 4; ++d) {
        int nr = r + DR[d], nc = c + DC[d];
        if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
        if (grid[nr][nc] == grid[r][c] + 1) paths += walk(nr, nc, peaks);
    }
    return paths;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) grid.push_back(line);
    h = grid.size();
    w = h ? grid[0].size() : 0;

    long long score = 0, rating = 0;
    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (grid[r][c] == '0') {
                std::set<std::pair<int, int>> peaks;
                rating += walk(r, c, peaks);
                score += peaks.size();
            }

    std::cout << "Czesc 1: " << score << '\n';
    std::cout << "Czesc 2: " << rating << '\n';
}
