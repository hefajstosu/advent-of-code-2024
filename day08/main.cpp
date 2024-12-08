#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
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

    // grupujemy anteny
    std::map<char, std::vector<std::pair<int, int>>> antennas;
    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c)
            if (grid[r][c] != '.') antennas[grid[r][c]].push_back({r, c});

    auto inside = [&](int r, int c) { return r >= 0 && r < h && c >= 0 && c < w; };
    std::set<std::pair<int, int>> part1, part2;

    for (auto& [freq, list] : antennas) {
        for (size_t i = 0; i < list.size(); ++i) {
            for (size_t j = 0; j < list.size(); ++j) {
                if (i == j) continue;
                auto [r1, c1] = list[i];
                auto [r2, c2] = list[j];
                int dr = r2 - r1, dc = c2 - c1;

                // odbijamy antene
                if (inside(r2 + dr, c2 + dc)) part1.insert({r2 + dr, c2 + dc});

                // cala prosta
                int g = std::gcd(std::abs(dr), std::abs(dc));
                int sr = dr / g, sc = dc / g;
                for (int r = r1, c = c1; inside(r, c); r += sr, c += sc) part2.insert({r, c});
            }
        }
    }

    std::cout << "Czesc 1: " << part1.size() << '\n';
    std::cout << "Czesc 2: " << part2.size() << '\n';
}
