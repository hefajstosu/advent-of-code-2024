#include <cstdio>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Robot {
    long long px, py, vx, vy;
};

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<Robot> robots;
    std::string line;
    long long maxX = 0;
    while (std::getline(in, line)) {
        Robot r;
        if (std::sscanf(line.c_str(), "p=%lld,%lld v=%lld,%lld", &r.px, &r.py, &r.vx, &r.vy) == 4) {
            robots.push_back(r);
            if (r.px > maxX) maxX = r.px;
        }
    }

    // zgadujemy rozmiar planszy
    const long long W = maxX < 11 ? 11 : 101;
    const long long H = maxX < 11 ? 7 : 103;

    auto mod = [](long long a, long long m) { return ((a % m) + m) % m; };

    // liczymy cwiartki
    long long q[4] = {0, 0, 0, 0};
    for (const Robot& r : robots) {
        long long x = mod(r.px + 100 * r.vx, W);
        long long y = mod(r.py + 100 * r.vy, H);
        if (x == W / 2 || y == H / 2) continue;
        q[(x > W / 2 ? 1 : 0) + (y > H / 2 ? 2 : 0)]++;
    }
    long long safety = q[0] * q[1] * q[2] * q[3];

    // szukamy choinki
    long long tree = -1;
    for (long long t = 0; t < W * H && tree < 0; ++t) {
        std::vector<char> seen(W * H, 0);
        bool ok = true;
        for (const Robot& r : robots) {
            long long x = mod(r.px + t * r.vx, W);
            long long y = mod(r.py + t * r.vy, H);
            if (seen[y * W + x]) { ok = false; break; }
            seen[y * W + x] = 1;
        }
        if (ok) tree = t;
    }

    std::cout << "Czesc 1: " << safety << '\n';
    std::cout << "Czesc 2: " << tree << '\n';
}
