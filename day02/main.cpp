#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// sprawdzamy raport
static bool isSafe(const std::vector<int>& v) {
    if (v.size() < 2) return true;
    bool increasing = v[1] > v[0];
    for (size_t i = 1; i < v.size(); ++i) {
        int d = v[i] - v[i - 1];
        if (!increasing) d = -d;
        if (d < 1 || d > 3) return false;
    }
    return true;
}

// wywalamy jeden poziom
static bool isSafeWithDampener(const std::vector<int>& v) {
    if (isSafe(v)) return true;
    for (size_t skip = 0; skip < v.size(); ++skip) {
        std::vector<int> w;
        w.reserve(v.size() - 1);
        for (size_t i = 0; i < v.size(); ++i)
            if (i != skip) w.push_back(v[i]);
        if (isSafe(w)) return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    long long safe = 0, safeDampened = 0;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::vector<int> levels;
        int x;
        while (ss >> x) levels.push_back(x);
        if (levels.empty()) continue;
        if (isSafe(levels)) ++safe;
        if (isSafeWithDampener(levels)) ++safeDampened;
    }

    std::cout << "Czesc 1: " << safe << '\n';
    std::cout << "Czesc 2: " << safeDampened << '\n';
}
