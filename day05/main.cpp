#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    // wczytujemy reguly
    std::set<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        size_t bar = line.find('|');
        if (bar != std::string::npos) {
            rules.insert({std::stoi(line.substr(0, bar)), std::stoi(line.substr(bar + 1))});
        } else {
            std::vector<int> pages;
            std::istringstream ss(line);
            std::string tok;
            while (std::getline(ss, tok, ',')) pages.push_back(std::stoi(tok));
            updates.push_back(pages);
        }
    }

    // porownujemy pary
    auto before = [&](int a, int b) { return rules.count({a, b}) > 0; };

    long long correctSum = 0, fixedSum = 0;
    for (auto& u : updates) {
        if (std::is_sorted(u.begin(), u.end(), before)) {
            correctSum += u[u.size() / 2];
        } else {
            std::sort(u.begin(), u.end(), before);
            fixedSum += u[u.size() / 2];
        }
    }

    std::cout << "Czesc 1: " << correctSum << '\n';
    std::cout << "Czesc 2: " << fixedSum << '\n';
}
