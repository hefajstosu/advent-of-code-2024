#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
    auto at = [&](int r, int c) -> char {
        if (r < 0 || r >= rows || c < 0 || c >= static_cast<int>(g[r].size())) return '.';
        return g[r][c];
    };

    // szukamy XMAS
    const std::string word = "XMAS";
    long long count = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < static_cast<int>(g[r].size()); ++c)
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    bool ok = true;
                    for (int k = 0; k < 4 && ok; ++k)
                        ok = at(r + dr * k, c + dc * k) == word[k];
                    if (ok) ++count;
                }

    // krzyzyki MAS
    long long xCount = 0;
    auto isMS = [](char a, char b) {
        return (a == 'M' && b == 'S') || (a == 'S' && b == 'M');
    };
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < static_cast<int>(g[r].size()); ++c) {
            if (g[r][c] != 'A') continue;
            if (isMS(at(r - 1, c - 1), at(r + 1, c + 1)) &&
                isMS(at(r - 1, c + 1), at(r + 1, c - 1)))
                ++xCount;
        }

    std::cout << "Czesc 1: " << count << '\n';
    std::cout << "Czesc 2: " << xCount << '\n';
}
