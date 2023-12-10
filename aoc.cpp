#include "aoc.h"
#include <cctype>

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


void aoc4() {
    vector<set<int>> symbols;
    vector<set<int>> stars;
    struct number {
        int start, end;
        int value;
    };

    vector<map<int, number>> numbers;
    vector<pair<int, int>> intervals;

    for (std::string line; std::getline(std::cin, line);) {
        symbols.push_back(set<int>());
        stars.push_back(set<int>());
        numbers.push_back(map<int, number>());

        for (auto i = 0; i < line.size(); i++) {
            // find a number's start and end
            if (std::isdigit(line[i])) {
                auto j = i;
                while (j < line.size() && std::isdigit(line[j])) {
                    // not clear if a number is a symbol but gives the 
                    // correct answer nonetheless...
                    symbols.back().insert(j);
                    j++;
                }

                auto num = stoi(line.substr(i, j - i));
                numbers.back()[i] = {i, j - 1, num};
                intervals.push_back({i, j - 1});
                i = j-1;
            } else if (line[i] != '.') {
                // or add a symbol if it's not a dot
                symbols.back().insert(i);
                if (line[i] == '*') {
                    stars.back().insert(i);
                }
            }
        }
    }

    /*
    std::cout <<"--------" << std::endl;
    for (auto i = 0; i < symbols.size(); i++) {
        for (auto j : symbols[i]) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout <<"--------" << std::endl;
    */

    auto is_symbol = [&](int i, int j) -> bool {
        if (i < 0 || i >= symbols.size()) {
            return false;
        }

        return symbols[i].find(j) != symbols[i].end();
    };

    auto has_symbol_in_neighbor_rows = [&](int i, int j) -> bool {
        return is_symbol(i-1, j-1) || is_symbol(i-1, j) || is_symbol(i-1, j+1)
            || is_symbol(i+1, j-1) || is_symbol(i+1, j) || is_symbol(i+1, j+1);
    };

    auto sum = 0;
    for (auto i = 0; i < numbers.size(); i++) {
        for (auto kv : numbers[i]) {
            auto num = kv.second;
            auto start = num.start;
            auto end = num.end;
            auto value = num.value;

            // start - 1, end + 1
            if (is_symbol(i, start - 1) || is_symbol(i, end + 1)) {
                sum += value;
                continue;
            }

            // previous && next rows
            auto adjacent_symbol = false;
            for (int j = start; j <= end; j++) {
                if (has_symbol_in_neighbor_rows(i, j)) {
                    adjacent_symbol = true;
                    break;
                }
            }

            if (adjacent_symbol) {
                sum += value;
            }
        }
    }


    // neighboring_numbers returns the numbers that are in the i'th row next to
    // the j'th column.
    auto neighboring_numbers = [&](int i, int j, vector<int>& result) {
        if (i < 0 || i >= numbers.size()) {
            return;
        }

        for (auto kv : numbers[i]) {
            auto num = kv.second;
            auto start = num.start;
            auto end = num.end;
            auto value = num.value;

            if ((start <= j && j <= end) || (start == j + 1) || (end == j - 1)) {
                result.push_back(value);
            }
        }
    };

    std::cout << sum << std::endl;

    auto sum_gear_rations = 0;
    for (auto i = 0; i < stars.size(); i++) {
        for (auto j : stars[i]) {
            vector<int> neighbors;
            neighboring_numbers(i-1, j, neighbors);
            neighboring_numbers(i, j, neighbors);
            neighboring_numbers(i+1, j, neighbors);
          
            if (neighbors.size() == 2) {
                sum_gear_rations += neighbors[0] * neighbors[1];
            }
        }
    }

    std::cout << sum_gear_rations << std::endl;
}

void aoc5() {
    auto sum = 0;
    map<int, int> copies;
    int curr = 0;
    int num_scratchcards = 0;

    for (std::string line; std::getline(std::cin, line);) {
        auto cols = tokenize(tokenize(line, ':')[1], '|');
        auto winning = tokenize(trim(cols[0]), ' ');
        auto cards = tokenize(trim(cols[1]), ' ');


        auto in_hand = unordered_map<int, int>();
        for (auto c : cards) {
            if (c == "") {
                continue;
            }
            in_hand[stoi(trim(c))]++;
        }

        auto power = -1;
        for (auto w : winning) {
            if (w == "") {
                continue;
            }
            auto it = in_hand.find(stoi(trim(w)));
            if (it != in_hand.end()) {
                power += 1; // it->second;
            }
        }
  
        auto curr_copies = 0;
        num_scratchcards++;
        if (copies.find(curr) != copies.end()) {
            num_scratchcards += copies[curr];
            curr_copies = copies[curr];
        }

        if (power > -1) {
            auto score = pow(2, power);
            sum += score;
    
            for (int i = 1; i <= power+1; i++) {
                copies[curr+i] += 1 + curr_copies;
            }
        }
        curr++;
    }

    cout << sum << endl;
    cout << num_scratchcards << endl;
}


int main() {
    // Problem set 1, test (0.txt)
    // Part 1 (1.txt)
    // aoc1();
    // Part 2 (2.txt)
    // aoc2();

    // Problem set 2, test (3.txt)
    // Part 1 (4.txt) => 2486
    // Part 2 (5.txt) => 87984
    // aoc3();

    // Problem set 3, test (6.txt) => 4361
    // Part 1 (7.txt) => 539713
    // Part 2 (7.txt) => 84159075
    // aoc4();

    // Problem set 4, test (8.txt) => 13
    // Part 1 (9.txt) => 20107
    // Part 1 (9.txt) => 8172507
    aoc5();
}
