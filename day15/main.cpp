#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using Grid = std::vector<std::string>;

// pchamy skrzynki
long long simulate(Grid g, const std::string& moves) {
    int rr = 0, rc = 0;
    for (int r = 0; r < (int)g.size(); ++r)
        for (int c = 0; c < (int)g[r].size(); ++c)
            if (g[r][c] == '@') { rr = r; rc = c; }

    for (char m : moves) {
        int dr = 0, dc = 0;
        if (m == '^') dr = -1;
        else if (m == 'v') dr = 1;
        else if (m == '<') dc = -1;
        else if (m == '>') dc = 1;
        else continue;

        std::vector<std::pair<int, int>> cells = {{rr, rc}};
        bool ok = true;
        for (size_t i = 0; i < cells.size() && ok; ++i) {
            int nr = cells[i].first + dr, nc = cells[i].second + dc;
            char ch = g[nr][nc];
            auto add = [&](int r, int c) {
                if (std::find(cells.begin(), cells.end(), std::make_pair(r, c)) == cells.end())
                    cells.push_back({r, c});
            };
            if (ch == '#') ok = false;
            else if (ch == 'O') add(nr, nc);
            else if (ch == '[') { add(nr, nc); if (dr != 0) add(nr, nc + 1); }
            else if (ch == ']') { add(nr, nc); if (dr != 0) add(nr, nc - 1); }
        }
        if (!ok) continue;

        std::vector<char> saved;
        for (auto [r, c] : cells) { saved.push_back(g[r][c]); g[r][c] = '.'; }
        for (size_t i = 0; i < cells.size(); ++i)
            g[cells[i].first + dr][cells[i].second + dc] = saved[i];
        rr += dr;
        rc += dc;
    }

    long long sum = 0;
    for (int r = 0; r < (int)g.size(); ++r)
        for (int c = 0; c < (int)g[r].size(); ++c)
            if (g[r][c] == 'O' || g[r][c] == '[') sum += 100LL * r + c;
    return sum;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    Grid grid;
    std::string line, moves;
    while (std::getline(in, line) && !line.empty()) grid.push_back(line);
    while (std::getline(in, line)) moves += line;

    // poszerzamy mape
    Grid wide;
    for (const std::string& row : grid) {
        std::string w;
        for (char ch : row) {
            if (ch == '#') w += "##";
            else if (ch == 'O') w += "[]";
            else if (ch == '@') w += "@.";
            else w += "..";
        }
        wide.push_back(w);
    }

    std::cout << "Czesc 1: " << simulate(grid, moves) << '\n';
    std::cout << "Czesc 2: " << simulate(wide, moves) << '\n';
}
