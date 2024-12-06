#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static const int DR[4] = {-1, 0, 1, 0};
static const int DC[4] = {0, 1, 0, -1};

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

    const int rows = static_cast<int>(g.size());
    const int cols = static_cast<int>(g[0].size());
    int sr = 0, sc = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (g[r][c] == '^') { sr = r; sc = c; }

    // chodzimy straznikiem
    std::vector<char> visited(rows * cols, 0);
    {
        int r = sr, c = sc, d = 0;
        while (true) {
            visited[r * cols + c] = 1;
            int nr = r + DR[d], nc = c + DC[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) break;
            if (g[nr][nc] == '#') d = (d + 1) % 4;
            else { r = nr; c = nc; }
        }
    }
    long long visitedCount = 0;
    for (char v : visited) visitedCount += v;

    // brute force petli
    std::vector<int> seen(rows * cols * 4, 0);
    int stamp = 0;
    auto loops = [&]() {
        ++stamp;
        int r = sr, c = sc, d = 0;
        while (true) {
            int key = (r * cols + c) * 4 + d;
            if (seen[key] == stamp) return true;
            seen[key] = stamp;
            int nr = r + DR[d], nc = c + DC[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) return false;
            if (g[nr][nc] == '#') d = (d + 1) % 4;
            else { r = nr; c = nc; }
        }
    };

    long long loopCount = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            if (!visited[r * cols + c] || (r == sr && c == sc)) continue;
            g[r][c] = '#';
            if (loops()) ++loopCount;
            g[r][c] = '.';
        }

    std::cout << "Czesc 1: " << visitedCount << '\n';
    std::cout << "Czesc 2: " << loopCount << '\n';
}
