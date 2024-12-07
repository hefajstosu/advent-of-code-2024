#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// sklejamy liczby
static long long concat(long long a, long long b) {
    long long m = 10;
    while (m <= b) m *= 10;
    return a * m + b;
}

// rekurencja po operatorach
static bool solvable(long long target, const std::vector<long long>& v, size_t i,
                     long long cur, bool allowConcat) {
    if (cur > target) return false;  // za duzo, odpada
    if (i == v.size()) return cur == target;
    if (solvable(target, v, i + 1, cur + v[i], allowConcat)) return true;
    if (solvable(target, v, i + 1, cur * v[i], allowConcat)) return true;
    return allowConcat && solvable(target, v, i + 1, concat(cur, v[i]), allowConcat);
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    long long sum1 = 0, sum2 = 0;
    std::string line;
    while (std::getline(in, line)) {
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;
        long long target = std::stoll(line.substr(0, colon));
        std::istringstream ss(line.substr(colon + 1));
        std::vector<long long> nums;
        long long x;
        while (ss >> x) nums.push_back(x);
        if (nums.empty()) continue;

        if (solvable(target, nums, 1, nums[0], false)) {
            sum1 += target;
            sum2 += target;
        } else if (solvable(target, nums, 1, nums[0], true)) {
            sum2 += target;
        }
    }

    std::cout << "Czesc 1: " << sum1 << '\n';
    std::cout << "Czesc 2: " << sum2 << '\n';
}
