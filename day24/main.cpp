#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct Gate {
    std::string a, op, b, out;
};

std::map<std::string, int> values;
std::map<std::string, Gate> byOutput;

int eval(const std::string& wire) {
    auto it = values.find(wire);
    if (it != values.end()) return it->second;
    const Gate& g = byOutput.at(wire);
    int x = eval(g.a), y = eval(g.b);
    int r = g.op == "AND" ? (x & y) : g.op == "OR" ? (x | y) : (x ^ y);
    return values[wire] = r;
}

bool isInput(const std::string& w) { return w[0] == 'x' || w[0] == 'y'; }

int main(int argc, char* argv[]) {
    const char* path = argc > 1 ? argv[1] : "data.txt";
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Nie mozna otworzyc pliku: " << path << '\n';
        return 1;
    }

    std::vector<Gate> gates;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            values[line.substr(0, colon)] = std::stoi(line.substr(colon + 1));
        } else {
            Gate g;
            std::string arrow;
            std::istringstream ss(line);
            ss >> g.a >> g.op >> g.b >> arrow >> g.out;
            gates.push_back(g);
            byOutput[g.out] = g;
        }
    }

    // skladamy liczbe z
    std::vector<std::string> zs;
    for (const auto& g : gates)
        if (g.out[0] == 'z') zs.push_back(g.out);
    std::sort(zs.begin(), zs.end());
    unsigned long long result = 0;
    for (int i = (int)zs.size() - 1; i >= 0; --i) result = (result << 1) | eval(zs[i]);

    // szukamy zlych bramek
    const std::string zLast = zs.empty() ? "" : zs.back();
    auto feedsInto = [&](const std::string& wire, const std::string& op) {
        for (const auto& g : gates)
            if (g.op == op && (g.a == wire || g.b == wire)) return true;
        return false;
    };
    std::set<std::string> wrong;
    for (const auto& g : gates) {
        bool firstBit = g.a.substr(1) == "00" && isInput(g.a);
        // z-ki z XOR
        if (g.out[0] == 'z' && g.op != "XOR" && g.out != zLast) wrong.insert(g.out);
        // XOR bez x/y
        if (g.op == "XOR" && g.out[0] != 'z' && !isInput(g.a) && !isInput(g.b)) wrong.insert(g.out);
        // XOR x/y zasila
        if (g.op == "XOR" && isInput(g.a) && isInput(g.b) && !firstBit && !feedsInto(g.out, "XOR"))
            wrong.insert(g.out);
        // AND do OR
        if (g.op == "AND" && !firstBit && !feedsInto(g.out, "OR")) wrong.insert(g.out);
    }
    std::string answer;
    for (const auto& w : wrong) {
        if (!answer.empty()) answer += ',';
        answer += w;
    }

    std::cout << "Czesc 1: " << result << '\n';
    std::cout << "Czesc 2: " << answer << '\n';
}
