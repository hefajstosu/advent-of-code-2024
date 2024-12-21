#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Pos {
    int r, c;
};

// klawiatura numeryczna
Pos numPos(char ch) {
    switch (ch) {
        case '7': return {0, 0};
        case '8': return {0, 1};
        case '9': return {0, 2};
        case '4': return {1, 0};
        case '5': return {1, 1};
        case '6': return {1, 2};
        case '1': return {2, 0};
        case '2': return {2, 1};
        case '3': return {2, 2};
        case '0': return {3, 1};
        default: return {3, 2};  // przycisk A
    }
}

// klawiatura kierunkowa
Pos dirPos(char ch) {
    switch (ch) {
        case '^': return {0, 1};
        case '<': return {1, 0};
        case 'v': return {1, 1};
        case '>': return {1, 2};
        default: return {0, 2};  // przycisk A
    }
}

int dirIndex(char ch) {
    switch (ch) {
        case '^': return 0;
        case '<': return 1;
        case 'v': return 2;
        case '>': return 3;
        default: return 4;  // przycisk A
    }
}

// kandydaci bez luki
std::vector<std::string> paths(Pos a, Pos b, Pos gap) {
    std::string hor(std::abs(b.c - a.c), b.c > a.c ? '>' : '<');
    std::string ver(std::abs(b.r - a.r), b.r > a.r ? 'v' : '^');
    std::vector<std::string> res;
    if (!(a.r == gap.r && b.c == gap.c)) res.push_back(hor + ver + "A");
    if (!(b.r == gap.r && a.c == gap.c)) res.push_back(ver + hor + "A");
    return res;
}

long long memo[30][5][5];

long long seqCost(const std::string& seq, int depth);

// koszt robota
long long dirCost(char a, char b, int depth) {
    if (depth == 0) return 1;
    long long& m = memo[depth][dirIndex(a)][dirIndex(b)];
    if (m >= 0) return m;
    long long best = -1;
    for (const auto& p : paths(dirPos(a), dirPos(b), {0, 0})) {
        long long cost = seqCost(p, depth - 1);
        if (best < 0 || cost < best) best = cost;
    }
    return m = best;
}

// koszt sekwencji
long long seqCost(const std::string& seq, int depth) {
    long long total = 0;
    char prev = 'A';
    for (char ch : seq) {
        total += dirCost(prev, ch, depth);
        prev = ch;
    }
    return total;
}

// dlugosc dla kodu
long long codeLength(const std::string& code, int robots) {
    long long total = 0;
    char prev = 'A';
    for (char ch : code) {
        long long best = -1;
        for (const auto& p : paths(numPos(prev), numPos(ch), {3, 0})) {
            long long cost = seqCost(p, robots);
            if (best < 0 || cost < best) best = cost;
        }
        total += best;
        prev = ch;
    }
    return total;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<std::string> codes;
    std::string line;
    while (in >> line) codes.push_back(line);

    std::memset(memo, -1, sizeof(memo));
    auto complexity = [&](int robots) {
        long long sum = 0;
        for (const auto& code : codes)
            sum += codeLength(code, robots) * std::stoll(code.substr(0, code.size() - 1));
        return sum;
    };

    std::cout << "Czesc 1: " << complexity(2) << '\n';
    std::cout << "Czesc 2: " << complexity(25) << '\n';
}
