#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Machine { long long ax, ay, bx, by, px, py; };

// Cramer, tylko calkowite
long long cost(const Machine& m, long long offset, long long limit) {
    long long px = m.px + offset, py = m.py + offset;
    long long det = m.ax * m.by - m.ay * m.bx;
    if (det == 0) return 0;  // rownolegle, odpada
    long long na = px * m.by - py * m.bx;
    long long nb = m.ax * py - m.ay * px;
    if (na % det != 0 || nb % det != 0) return 0;
    long long a = na / det, b = nb / det;
    if (a < 0 || b < 0) return 0;
    if (limit > 0 && (a > limit || b > limit)) return 0;
    return 3 * a + b;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::stringstream ss;
    ss << in.rdbuf();
    std::string text = ss.str();

    std::vector<Machine> machines;
    Machine m;
    const char* p = text.c_str();
    int consumed = 0;
    while (std::sscanf(p, " Button A: X+%lld, Y+%lld Button B: X+%lld, Y+%lld Prize: X=%lld, Y=%lld%n",
                       &m.ax, &m.ay, &m.bx, &m.by, &m.px, &m.py, &consumed) == 6) {
        machines.push_back(m);
        p += consumed;
    }

    long long part1 = 0, part2 = 0;
    for (const Machine& mm : machines) {
        part1 += cost(mm, 0, 100);
        part2 += cost(mm, 10000000000000LL, 0);
    }

    std::cout << "Czesc 1: " << part1 << '\n';
    std::cout << "Czesc 2: " << part2 << '\n';
}
