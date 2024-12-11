#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using Counts = std::unordered_map<long long, long long>;

// mrugamy zbiorczo
Counts blink(const Counts& stones) {
    Counts next;
    for (auto& [v, n] : stones) {
        if (v == 0) {
            next[1] += n;
            continue;
        }
        std::string s = std::to_string(v);
        if (s.size() % 2 == 0) {
            size_t half = s.size() / 2;
            next[std::stoll(s.substr(0, half))] += n;
            next[std::stoll(s.substr(half))] += n;
        } else {
            next[v * 2024] += n;
        }
    }
    return next;
}

long long total(const Counts& stones) {
    long long sum = 0;
    for (auto& [v, n] : stones) sum += n;
    return sum;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    Counts stones;
    long long v;
    while (in >> v) ++stones[v];

    for (int i = 0; i < 25; ++i) stones = blink(stones);
    std::cout << "Czesc 1: " << total(stones) << '\n';
    for (int i = 25; i < 75; ++i) stones = blink(stones);
    std::cout << "Czesc 2: " << total(stones) << '\n';
}
