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

    std::string map;
    std::getline(in, map);
    while (!map.empty() && (map.back() < '0' || map.back() > '9')) map.pop_back();

    // przesuwamy bloki
    std::vector<int> blocks;  // -1 to dziura
    for (size_t i = 0; i < map.size(); ++i) {
        int len = map[i] - '0';
        int id = (i % 2 == 0) ? static_cast<int>(i / 2) : -1;
        blocks.insert(blocks.end(), len, id);
    }
    {
        std::vector<int> disk = blocks;
        size_t l = 0, r = disk.size();
        while (true) {
            while (l < disk.size() && disk[l] != -1) ++l;
            while (r > 0 && disk[r - 1] == -1) --r;
            if (r == 0 || l >= r - 1) break;
            disk[l] = disk[r - 1];
            disk[r - 1] = -1;
        }
        long long checksum = 0;
        for (size_t i = 0; i < disk.size(); ++i)
            if (disk[i] != -1) checksum += static_cast<long long>(i) * disk[i];
        std::cout << "Czesc 1: " << checksum << '\n';
    }

    // przesuwamy cale pliki
    struct Span { long long pos, len; };
    std::vector<Span> files, gaps;
    long long pos = 0;
    for (size_t i = 0; i < map.size(); ++i) {
        int len = map[i] - '0';
        if (i % 2 == 0) files.push_back({pos, len});
        else gaps.push_back({pos, len});
        pos += len;
    }
    for (int id = static_cast<int>(files.size()) - 1; id >= 0; --id) {
        Span& f = files[id];
        for (Span& g : gaps) {
            if (g.pos >= f.pos) break;
            if (g.len >= f.len) {
                f.pos = g.pos;
                g.pos += f.len;
                g.len -= f.len;
                break;
            }
        }
    }
    long long checksum = 0;
    for (size_t id = 0; id < files.size(); ++id)
        for (long long k = 0; k < files[id].len; ++k)
            checksum += (files[id].pos + k) * static_cast<long long>(id);
    std::cout << "Czesc 2: " << checksum << '\n';
}
