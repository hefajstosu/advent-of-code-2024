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

    // wczytujemy schematy
    std::vector<std::string> locks, keys;
    std::string line, block;
    auto flush = [&]() {
        if (block.empty()) return;
        // zamek czy klucz
        (block[0] == '#' ? locks : keys).push_back(block);
        block.clear();
    };
    while (std::getline(in, line)) {
        if (line.empty()) flush();
        else block += line;
    }
    flush();

    // sprawdzamy dopasowanie
    long long fits = 0;
    for (const auto& l : locks) {
        for (const auto& k : keys) {
            bool ok = l.size() == k.size();
            for (size_t i = 0; ok && i < l.size(); ++i)
                if (l[i] == '#' && k[i] == '#') ok = false;
            if (ok) ++fits;
        }
    }

    std::cout << "Czesc 1: " << fits << '\n';
    // brak czesci 2
    std::cout << "Czesc 2: -" << '\n';
}
