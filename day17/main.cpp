#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using u64 = unsigned long long;

// interpreter kompa
std::vector<int> run(u64 A, u64 B, u64 C, const std::vector<int>& prog) {
    std::vector<int> out;
    size_t ip = 0;
    auto shr = [](u64 v, u64 s) { return s >= 64 ? 0ULL : v >> s; };
    while (ip + 1 < prog.size()) {
        int op = prog[ip], lit = prog[ip + 1];
        u64 combo = lit <= 3 ? lit : lit == 4 ? A : lit == 5 ? B : C;
        ip += 2;
        switch (op) {
            case 0: A = shr(A, combo); break;          // adv, przesuwamy A
            case 1: B ^= lit; break;                    // bxl, xorujemy B
            case 2: B = combo % 8; break;               // bst, modulo 8
            case 3: if (A != 0) ip = lit; break;        // jnz, skaczemy
            case 4: B ^= C; break;                      // bxc, xorujemy C
            case 5: out.push_back(combo % 8); break;    // out, wypluwamy
            case 6: B = shr(A, combo); break;           // bdv, wynik do B
            case 7: C = shr(A, combo); break;           // cdv, wynik do C
        }
    }
    return out;
}

// skladamy A wstecz
long long findA(u64 prefix, size_t matched, u64 B, u64 C, const std::vector<int>& prog) {
    if (matched == prog.size()) return (long long)prefix;
    for (u64 k = 0; k < 8; ++k) {
        u64 a = (prefix << 3) | k;
        if (a == 0) continue;  // zero odpada
        std::vector<int> out = run(a, B, C, prog);
        std::vector<int> suffix(prog.end() - (matched + 1), prog.end());
        if (out == suffix) {
            long long r = findA(a, matched + 1, B, C, prog);
            if (r >= 0) return r;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    u64 A = 0, B = 0, C = 0;
    std::vector<int> prog;
    std::string line;
    while (std::getline(in, line)) {
        if (std::sscanf(line.c_str(), "Register A: %llu", &A) == 1) continue;
        if (std::sscanf(line.c_str(), "Register B: %llu", &B) == 1) continue;
        if (std::sscanf(line.c_str(), "Register C: %llu", &C) == 1) continue;
        if (line.rfind("Program:", 0) == 0) {
            std::stringstream ss(line.substr(8));
            std::string tok;
            while (std::getline(ss, tok, ',')) prog.push_back(std::stoi(tok));
        }
    }

    // wypisujemy wyjscie
    std::vector<int> out = run(A, B, C, prog);
    std::string s;
    for (size_t i = 0; i < out.size(); ++i) s += (i ? "," : "") + std::to_string(out[i]);

    std::cout << "Czesc 1: " << s << '\n';
    std::cout << "Czesc 2: " << findA(0, 0, B, C, prog) << '\n';
}
