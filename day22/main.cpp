#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

long long nextSecret(long long x) {
    const long long mod = 16777216;
    x = (x ^ (x * 64)) % mod;
    x = (x ^ (x / 32)) % mod;
    x = (x ^ (x * 2048)) % mod;
    return x;
}

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<long long> buyers;
    long long v;
    while (in >> v) buyers.push_back(v);

    // kodujemy sekwencje
    const int SEQS = 19 * 19 * 19 * 19;
    std::vector<long long> bananas(SEQS, 0);
    std::vector<int> seenBy(SEQS, -1);

    long long sumSecrets = 0;
    for (int b = 0; b < (int)buyers.size(); ++b) {
        long long x = buyers[b];
        int prevPrice = x % 10;
        int key = 0;
        for (int i = 1; i <= 2000; ++i) {
            x = nextSecret(x);
            int price = x % 10;
            key = (key * 19 + (price - prevPrice + 9)) % SEQS;
            prevPrice = price;
            // tylko pierwsze wystapienie
            if (i >= 4 && seenBy[key] != b) {
                seenBy[key] = b;
                bananas[key] += price;
            }
        }
        sumSecrets += x;
    }

    std::cout << "Czesc 1: " << sumSecrets << '\n';
    std::cout << "Czesc 2: " << *std::max_element(bananas.begin(), bananas.end()) << '\n';
}
