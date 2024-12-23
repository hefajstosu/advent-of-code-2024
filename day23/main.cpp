#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const int N = 26 * 26;
using Set = std::bitset<N>;

Set adj[N];
Set best;

// Bron-Kerbosch, max klika
void bronKerbosch(Set r, Set p, Set x) {
    if (p.none() && x.none()) {
        if (r.count() > best.count()) best = r;
        return;
    }
    if (r.count() + p.count() <= best.count()) return;
    Set px = p | x;
    int pivot = 0;
    size_t pivotDeg = 0;
    for (int u = 0; u < N; ++u) {
        if (!px[u]) continue;
        size_t deg = (p & adj[u]).count();
        if (deg >= pivotDeg) pivotDeg = deg, pivot = u;
    }
    Set candidates = p & ~adj[pivot];
    for (int v = 0; v < N; ++v) {
        if (!candidates[v]) continue;
        Set nr = r;
        nr.set(v);
        bronKerbosch(nr, p & adj[v], x & adj[v]);
        p.reset(v);
        x.set(v);
    }
}

int id(const std::string& s) { return (s[0] - 'a') * 26 + (s[1] - 'a'); }
std::string name(int i) { return std::string{char('a' + i / 26), char('a' + i % 26)}; }

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    Set nodes;
    std::string line;
    while (in >> line) {
        int a = id(line.substr(0, 2)), b = id(line.substr(3, 2));
        adj[a].set(b);
        adj[b].set(a);
        nodes.set(a);
        nodes.set(b);
    }

    // trojki z t
    auto isT = [](int i) { return i / 26 == 't' - 'a'; };
    long long triangles = 0;
    for (int a = 0; a < N; ++a) {
        if (!nodes[a]) continue;
        for (int b = a + 1; b < N; ++b) {
            if (!adj[a][b]) continue;
            Set common = adj[a] & adj[b];
            for (int c = b + 1; c < N; ++c)
                if (common[c] && (isT(a) || isT(b) || isT(c))) ++triangles;
        }
    }

    // skladamy haslo
    bronKerbosch(Set(), nodes, Set());
    std::string password;
    for (int i = 0; i < N; ++i) {
        if (!best[i]) continue;
        if (!password.empty()) password += ',';
        password += name(i);
    }

    std::cout << "Czesc 1: " << triangles << '\n';
    std::cout << "Czesc 2: " << password << '\n';
}
