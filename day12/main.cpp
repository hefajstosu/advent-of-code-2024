#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(in, line))
        if (!line.empty()) grid.push_back(line);
    int h = grid.size(), w = h ? grid[0].size() : 0;

    auto same = [&](int r, int c, char ch) {
        return r >= 0 && r < h && c >= 0 && c < w && grid[r][c] == ch;
    };
    const int DR[] = {-1, 0, 1, 0};
    const int DC[] = {0, 1, 0, -1};

    std::vector<std::vector<bool>> seen(h, std::vector<bool>(w, false));
    long long price1 = 0, price2 = 0;

    for (int sr = 0; sr < h; ++sr) {
        for (int sc = 0; sc < w; ++sc) {
            if (seen[sr][sc]) continue;
            char ch = grid[sr][sc];
            long long area = 0, perimeter = 0, corners = 0;
            std::vector<std::pair<int, int>> stack = {{sr, sc}};
            seen[sr][sc] = true;
            while (!stack.empty()) {
                auto [r, c] = stack.back();
                stack.pop_back();
                ++area;
                for (int d = 0; d < 4; ++d) {
                    int nr = r + DR[d], nc = c + DC[d];
                    if (!same(nr, nc, ch)) {
                        ++perimeter;
                    } else if (!seen[nr][nc]) {
                        seen[nr][nc] = true;
                        stack.push_back({nr, nc});
                    }
                    // liczymy narozniki
                    int e = (d + 1) % 4;
                    bool a = same(r + DR[d], c + DC[d], ch);
                    bool b = same(r + DR[e], c + DC[e], ch);
                    bool diag = same(r + DR[d] + DR[e], c + DC[d] + DC[e], ch);
                    if (!a && !b) ++corners;
                    if (a && b && !diag) ++corners;
                }
            }
            price1 += area * perimeter;
            price2 += area * corners;
        }
    }

    std::cout << "Czesc 1: " << price1 << '\n';
    std::cout << "Czesc 2: " << price2 << '\n';
}
