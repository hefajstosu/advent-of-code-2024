#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<long long> left, right;
    long long a, b;
    while (in >> a >> b) {
        left.push_back(a);
        right.push_back(b);
    }

    // sumujemy odleglosci
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    long long distance = 0;
    for (size_t i = 0; i < left.size(); ++i)
        distance += std::llabs(left[i] - right[i]);

    // liczymy podobienstwo
    std::unordered_map<long long, long long> count;
    for (long long x : right) ++count[x];
    long long similarity = 0;
    for (long long x : left) {
        auto it = count.find(x);
        if (it != count.end()) similarity += x * it->second;
    }

    std::cout << "Czesc 1: " << distance << '\n';
    std::cout << "Czesc 2: " << similarity << '\n';
}
