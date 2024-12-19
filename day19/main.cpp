#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::string line;
    std::getline(in, line);
    std::vector<std::string> towels;
    std::stringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        size_t b = tok.find_first_not_of(' ');
        if (b != std::string::npos) towels.push_back(tok.substr(b));
    }

    long long possible = 0, ways = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        // dp po prefiksach
        std::vector<long long> dp(line.size() + 1, 0);
        dp[0] = 1;
        for (size_t i = 0; i < line.size(); ++i) {
            if (!dp[i]) continue;
            for (const std::string& t : towels)
                if (line.compare(i, t.size(), t) == 0) dp[i + t.size()] += dp[i];
        }
        if (dp.back() > 0) ++possible;  // liczymy mozliwe
        ways += dp.back();              // sumujemy sposoby
    }

    std::cout << "Czesc 1: " << possible << '\n';
    std::cout << "Czesc 2: " << ways << '\n';
}
