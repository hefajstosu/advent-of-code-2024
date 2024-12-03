#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// czytamy liczbe
static bool readNumber(const std::string& s, size_t& i, long long& out) {
    size_t start = i;
    out = 0;
    while (i < s.size() && i - start < 3 && std::isdigit(static_cast<unsigned char>(s[i]))) {
        out = out * 10 + (s[i] - '0');
        ++i;
    }
    return i > start;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }
    std::stringstream buf;
    buf << in.rdbuf();
    const std::string s = buf.str();

    long long total = 0, enabledTotal = 0;
    bool enabled = true;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s.compare(i, 4, "do()") == 0) {
            enabled = true;
        } else if (s.compare(i, 7, "don't()") == 0) {
            enabled = false;
        } else if (s.compare(i, 4, "mul(") == 0) {
            // parsujemy mul
            size_t j = i + 4;
            long long x, y;
            if (!readNumber(s, j, x)) continue;
            if (j >= s.size() || s[j] != ',') continue;
            ++j;
            if (!readNumber(s, j, y)) continue;
            if (j >= s.size() || s[j] != ')') continue;
            total += x * y;
            if (enabled) enabledTotal += x * y;
        }
    }

    std::cout << "Czesc 1: " << total << '\n';
    std::cout << "Czesc 2: " << enabledTotal << '\n';
}
