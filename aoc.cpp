#include "aoc.h"

void aoc1() {
    int sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        int first = -1, last = -1;
        for (int i = 0, j = line.size() - 1; i < line.size() && j >= 0; ++i, --j) {
            if (first == -1 && std::isdigit(line[i])) {
                first = line[i] - '0';
            }

            if (last == -1 && std::isdigit(line[j])) {
                last = line[j] - '0';
            }

            if (first != -1 && last != -1) {
                break;
            }
        }
        sum += first * 10 + last;
    }

    std::cout << sum << std::endl;
}

void aoc2() {
    map<string, int> digits = {
        {"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, 
        {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, 
        {"eight", 8}, {"nine", 9},
    };

    auto is_digit = [](char c) -> int {
        if (std::isdigit(c)) {
            return static_cast<int>(c - '0');
        }
        return -1;
    };

    auto is_str_digit = [&](string s) -> int {
        for (auto d : digits) {
            auto ds = d.first;
            if (ds.size() <= s.size() && s.substr(0, ds.length()) == ds) {
                return d.second;
            }
        }

        return -1;
    };

    int sum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        int first = -1, last = -1;
        for (int i = 0, j = line.size() - 1; i < line.size() && j >= 0; ++i, --j) {
            if (first == -1) {
                first = is_digit(line[i]);
            }

            if (first == -1) {
                first = is_str_digit(line.substr(i));
            }

            if (last == -1) {
                last = is_digit(line[j]);
            }

            if (last == -1) {
                last = is_str_digit(line.substr(j));
            }
        }
        sum += first * 10 + last;
    }

    std::cout << sum << std::endl;
}


int main() {
    aoc2();
}
