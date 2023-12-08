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

vector<string> tokenize(const string &str, char delimiter) {
    vector<std::string> tokens;
    istringstream tokenStream(str);
    string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');

    if (first == string::npos || last == string::npos) {
        return "";
    }

    return str.substr(first, (last - first + 1));
}

void aoc3() {
    struct game_set {
        int red, green, blue;
    };

    auto make_game_set = [](const string &line) -> game_set {
        auto grabs = tokenize(line, ',');
        
        int red = 0, green = 0, blue = 0;
        for (auto g : grabs) {
            auto parts = tokenize(trim(g), ' ');
            auto count = stoi(trim(parts[0]));
            auto color = trim(parts[1]);
            switch (color[0]) {
                case 'r':
                    red += count;
                    break;
                case 'g':
                    green += count;
                    break;
                case 'b':
                    blue += count;
                    break;
            }
        }

        return {red, green, blue};
    };
    
    auto get_game_id = [](const string &line) -> int {
        auto tokens = tokenize(line, ':');
        auto parts = tokenize(tokens[0], ' ');
        return stoi(parts[1]);
    }; 

    auto get_sets = [&](const string &line) -> vector<game_set> {
        auto game = tokenize(line, ':');
        auto groups = tokenize(game[1], ';');
        vector<game_set> sets;
        for (auto g : groups) {
            sets.push_back(make_game_set(g));
        }
        return sets;
    };

    auto is_game_set_possible = [](const game_set &set) -> bool {
        return set.red <= 12 && set.green <= 13 && set.blue <= 14;
    };

    auto id_sum = 0;
    auto powersum = 0;
    for (std::string line; std::getline(std::cin, line);) {
        auto game_id = get_game_id(line);
        auto sets = get_sets(line);
        
        auto is_game_possible = true;
        for (auto s : sets) {
            if (!is_game_set_possible(s)) {
                is_game_possible = false;
                break;
            }
        }

        if (is_game_possible) {
            id_sum += game_id;
        }
        
        auto maxr = 1, maxg = 1, maxb = 1;
        for (auto s : sets) {
            maxr = std::max(maxr, s.red);
            maxg = std::max(maxg, s.green);
            maxb = std::max(maxb, s.blue);
        }

        powersum += maxr * maxg * maxb;
    }

    cout << id_sum << endl;
    cout << powersum << endl;
}


int main() {
    // Problem set 1, test (0.txt)
    // Part 1 (1.txt)
    // aoc1();
    // Part 2 (2.txt)
    // aoc2();

    // Problem set 2, test (3.txt)
    // Part 1 (4.txt) => 2486
    aoc3();
}
