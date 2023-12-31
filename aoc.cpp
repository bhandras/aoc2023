#include "aoc.h"
#include <cctype>
#include <numeric>

void aoc1(ifstream& fs) {
    int sum = 0;
    for (std::string line; std::getline(fs, line);) {
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

    cout << sum << endl;
}

void aoc1_part2(ifstream& fs) {
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
    for (std::string line; std::getline(fs, line);) {
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

    cout << sum << endl;
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

void aoc2(ifstream& fs) {
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
    for (std::string line; std::getline(fs, line);) {
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


void aoc3(ifstream& fs) {
    vector<set<int>> symbols;
    vector<set<int>> stars;
    struct number {
        int start, end;
        int value;
    };

    vector<map<int, number>> numbers;
    vector<pair<int, int>> intervals;

    for (std::string line; std::getline(fs, line);) {
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
    cout <<"--------" << endl;
    for (auto i = 0; i < symbols.size(); i++) {
        for (auto j : symbols[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout <<"--------" << endl;
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

    cout << sum << endl;

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

    cout << sum_gear_rations << endl;
}

void aoc4(ifstream& fs) {
    auto sum = 0;
    map<int, int> copies;
    int curr = 0;
    int num_scratchcards = 0;

    for (std::string line; std::getline(fs, line);) {
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

uint64_t read_uint64(string s) {
    std::istringstream iss(s);
    uint64_t val;
    iss >> val;

    return val;
}

void aoc5(ifstream& fs, bool seed_intervals) {
    struct mapping {
        struct interval {
            uint64_t src, dst, len;
        };

        vector<interval> intervals;
    };

    auto readMapping = [&](string name) -> mapping {
        string line;
        getline(fs, line);
        if (line != name) {
            throw std::runtime_error("invalid mapping");
        }

        mapping m;
        for (std::string line; std::getline(fs, line);) {
            if (line == "") {
                break;
            }

            auto tokens = tokenize(line, ' ');
            auto dst = read_uint64(trim(tokens[0]));
            auto src = read_uint64(trim(tokens[1]));
            auto len = read_uint64(trim(tokens[2]));
            m.intervals.push_back({src, dst, len});
        }

        return m;
    };

    // First line containing the seeds.
    string line;
    std::getline(fs, line);
    auto seedTokens = tokenize(tokenize(line, ':')[1], ' ');
    vector<uint64_t> seeds;

    for (auto s : seedTokens) {
        if (s == "") {
            continue;
        }

        auto seed = read_uint64(trim(s));
        seeds.push_back(seed);
        if (!seed_intervals) {
            seeds.push_back(1);
        }
    }

    // Read an empty line.
    getline(fs, line);

    // Read mappings.
    vector<mapping> mappings;
    mappings.push_back(readMapping("seed-to-soil map:"));
    mappings.push_back(readMapping("soil-to-fertilizer map:"));
    mappings.push_back(readMapping("fertilizer-to-water map:"));
    mappings.push_back(readMapping("water-to-light map:"));
    mappings.push_back(readMapping("light-to-temperature map:"));
    mappings.push_back(readMapping("temperature-to-humidity map:"));
    mappings.push_back(readMapping("humidity-to-location map:"));

    auto translate = [](const mapping& m, uint64_t val) -> uint64_t {
        for (const auto& interval : m.intervals) {
            if (val >= interval.src && val < interval.src + interval.len) {
                return interval.dst + (val - interval.src);
            }
        }

        return val;
    };

    set<pair<uint64_t, uint64_t>> current;
    // Translate seeds to  closed intervals;
    for (auto i = 0; i < seeds.size(); i += 2) {
        current.insert(make_pair(seeds[i], seeds[i] + seeds[i+1] - 1));
    }

    set<pair<uint64_t, uint64_t>> next;
    int level = 0;
    while (level < mappings.size()) {
        while (!current.empty()) {
            const auto& it = current.begin();

            // [l1, r1] is the next "seed" interval for the current level.
            const auto l1 = it->first;
            const auto r1 = it->second;

            // Now intersect it with the current level's mapping.
            auto has_intersection = false;
            for (const auto& interval : mappings[level].intervals) {
                const auto l2 = interval.src;
                const auto r2 = interval.src + interval.len - 1;

                if (r1 < l2 || r2 < l1) {
                    continue;
                }
               
                has_intersection = true;

                // The intersection is [max(l1, l2), min(r1, r2)]. 
                next.insert({std::max(l1, l2), std::min(r1, r2)});

                // If the seed interval's left is outside.
                if (l1 < l2) {
                    current.insert({l1, l2-1});
                }

                // If the seed interval's right is outside.
                if (r2 < r1) {
                    current.insert({r2+1, r1});
                }
            }

            // If there was no intersection, then the seed interval moves 
            // to the next level unchanged.
            if (!has_intersection) {
                next.insert({l1, r1});
            }

            // We can now delete the current seed interval since it's been
            // sliced up into smaller intervals.
            current.erase(it);
        }
        
      
        // Now translate the new intervals to the next level.
        for (const auto& v : next) {
            current.insert({
                    translate(mappings[level], v.first), 
                    translate(mappings[level], v.second)
            });
        }

        // Jump to the next level.
        next.clear();
        level++;
    }

    auto min_location = current.begin()->first;
    cout << min_location << endl;
}

void aoc6(ifstream& fs, bool no_kerning = false) {
    string line;

    getline(fs, line);
    auto tokens = tokenize(tokenize(line, ':')[1], ' ');
    string ts;
    vector<uint64_t> times;
    for (auto t : tokens) {
        if (t == "") {
            continue;
        }

        ts += t;
        times.push_back(read_uint64(t));
    }

    getline(fs, line);
    tokens = tokenize(tokenize(line, ':')[1], ' ');
    string ds;
    vector<uint64_t> distances;
    for (auto t : tokens) {
        if (t == "") {
            continue;
        }

        ds += t;
        distances.push_back(read_uint64(t));
    }

    if (no_kerning) {
        times = {read_uint64(ts)};
        distances = {read_uint64(ds)};
    }

    int total = 1;
    for (int i = 0; i < times.size(); i++) {
        int winning = 0;
        for (int hold_time = 1; hold_time < times[i]; hold_time++) {
            if ((times[i] - hold_time) * hold_time > distances[i]) {
                winning++;
            }
        }
        
        total *= winning;
    }

    cout << total << endl;
}

void aoc7(ifstream& fs) {
    struct card {
        char value;

        int score(bool joker) const {
            switch (value) {
                case 'A':
                    return 14;
                case 'K':
                    return 13;
                case 'Q':
                    return 12;
                case 'J':
                    if (joker) {
                        return 1;
                    }
                    return 11;
                case 'T':
                    return 10;
                default:
                    return value - '0';
            }
        }
    };

    struct hand {
        vector<card> cards;
        bool joker;

        hand(string s, bool joker) 
            : cards(vector<card>(s.size())), joker(joker) {
            for (int i = 0; i < 5; i++) {
                cards[i].value = s[i];
            }
        }

        int score() const {
            char max_card;
            int max_count = 0;
            unordered_map<char, int> counts;
            int count_j = 0;

            for (auto c : cards) {
                if (joker && c.value == 'J') {
                    count_j++;
                    continue;
                }

                counts[c.value]++;
                if (counts[c.value] > max_count) {
                    max_count = counts[c.value];
                    max_card = c.value;
                }
            }

            if (count_j > 0) {
                counts[max_card] += count_j;
                max_count += count_j;
            }

            // five of a kind gets 7 points
            if (max_count == 5) {
                return 7;
            }

            // four of a kind gets 6 points
            if (max_count == 4) {
                return 6;
            }

            if (max_count == 3) {
                // full house gets 5 points
                if (counts.size() == 2) {
                    return 5;
                }

                // three of a kind gets 4 points
                return 4;
            }

            if (max_count == 2) {
                // two pairs gets 3 points
                if (counts.size() == 3) {
                    return 3;
                }

                // one pair gets 2 point
                return 2;
            }

            if (counts.size() == 5) {
                // high card gets 1 point 
                return 1;
            }

            return 0;
        }

        bool operator <(const hand& other) const {
            auto s1 = score();
            auto s2 = other.score();
            if (s1 != s2) {
                return s1 < s2;
            }

            for (int i = 0; i < 5; i++) {
                if (cards[i].value != other.cards[i].value) {
                    return cards[i].score(joker) < other.cards[i].score(joker);
                }
            }

            return false;
        }

        void print() const {
            for (auto c : cards) {
                cout << c.value;
            } 
            cout << " score: " << score();
        }
    };

    map<hand, int> hands;
    map<hand, int> hands_j;
    for (std::string line; std::getline(fs, line);) {
        auto tokens = tokenize(line, ' ');
        auto bid = stoi(tokens[1]);
        hands.insert({hand(tokens[0], false), bid});
        hands_j.insert({hand(tokens[0], true), bid});
    }

    int rank = 1, total_winnings1 = 0;
    for (auto hand_bid : hands) {
        total_winnings1 += hand_bid.second * rank;
        rank++;
    }

    rank = 1;
    int total_winnings2 = 0;
    for (auto hand_bid : hands_j) {
        total_winnings2 += hand_bid.second * rank;
        rank++;
    }

    cout << "part1: " << total_winnings1 << endl 
         << "part2: " << total_winnings2 << endl;
}

uint64_t lcm(vector<uint64_t> v) {
    if (v.size() == 2) {
        return std::lcm(v[0], v[1]);
    }

    return std::lcm(v[0], lcm(vector<uint64_t>(v.begin() + 1, v.end())));
}

void aoc8(ifstream& fs, bool part2 = false) {
    string steps;
    getline(fs, steps);

    // Jump over an empty line.
    string line;
    getline(fs, line);

    // Read the mapping.
    unordered_map<string, pair<string, string>> mapping;

    for (std::string line; std::getline(fs, line);) {
        if (line == "") {
            break;
        }

        // We don't really need to tokenize as everything is fixed size.
        auto src = line.substr(0, 3);
        auto l = line.substr(7, 3);
        auto r = line.substr(12, 3);

        mapping[src] = {l, r};
    }

    vector<pair<string, int>> state;

    if (!part2) {
        state = {{"AAA", -1}};
    } else {
        for (const auto& kv : mapping) {
            if (kv.first[2] == 'A') {
                state.push_back({kv.first, -1});
            }
        }
    }


    auto step = [&](string curr, char step) -> string {
        switch (step) {
            case 'L':
                return mapping[curr].first;

            case 'R':
                return mapping[curr].second;

            default:
                cout << "invalid step" << endl;
                return "";
        }
    };
    
    cout << "start: ";
    for (auto i = 0; i < state.size(); i++) {
        cout << state[i].first << " ";
    }
    cout << endl;

    int cnt = 0;

    int have_z = 0;
    for (;;) {
        for (auto i = 0; i < steps.size(); i++) {
            cnt++;
            
            for (auto j = 0; j < state.size(); j++) {
                state[j].first = step(state[j].first, steps[i]);

                // For part 2 we find the step when each state starts to loop.
                if (part2 && state[j].first[2] == 'Z' && state[j].second == -1) {
                    state[j].second = cnt;
                    have_z++;
                }
            }

            if (!part2) {
                if (state[0].first == "ZZZ") {
                    cout << cnt << endl;
                    return;
                }
            } else {
                if (have_z == state.size()) {
                    vector<uint64_t> zs;
                    for (auto s : state) {
                        zs.push_back(s.second);
                    }

                    // And then we just calculate the least common multiple.
                    // This also assumes that the test data really loops after
                    // we find the first Zs for each starting state, which
                    // appears to be true.
                    cout << lcm(zs) << endl;
                    return;
                }
            }
        }
    }
}

void run(string filename, function<void(ifstream&)> fn) {
    std::ifstream fs(filename);
    fn(fs);
    fs.close();
}

int main() {
    /*
    // Problem set 1, test (0.txt) => 142 
    run("0.txt", aoc1);
    // Part 1 (1.txt) => 54877
    run("1.txt", aoc1);
    // Part 2 (2.txt) => 281
    run("2.txt", aoc1_part2); 
    // Part 2 (1.txt) => 
    run("1.txt", aoc1_part2); 

    // Problem set 2, test (3.txt) => 8, 2286
    run("3.txt", aoc2);
    // Part 1 (4.txt) => 2486
    // Part 2 (4.txt) => 87984
    run("4.txt", aoc2);

    // Problem set 3, test (6.txt) => 4361, 467835
    run("6.txt", aoc3);
    // Part 1 (7.txt) => 539713
    // Part 2 (7.txt) => 84159075
    run("7.txt", aoc3);

    // Problem set 4, test (8.txt) => 13, 30
    run("8.txt", aoc4);
    // Part 1 (9.txt) => 20107
    // Part 1 (9.txt) => 8172507
    run("9.txt", aoc4);

    // Problem set 5, test (10.txt) => 35
    run("10.txt", [](ifstream& fs) { aoc5(fs, false); });
    // Part 1 (11.txt) => 424490994
    run("11.txt", [](ifstream& fs) { aoc5(fs, false); });
    // Part 2 test (10.txt) => 46
    run("10.txt", [](ifstream& fs) { aoc5(fs, true); });
    // Part 2 (11.txt) => 15290096
    run("11.txt", [](ifstream& fs) { aoc5(fs, true); });

    // Problem set 6, test (12.txt) => 288
    run("12.txt", [](ifstream& fs) { aoc6(fs, false); });
    // Part 1 (13.txt) => 625968
    run("13.txt", [](ifstream& fs) { aoc6(fs, false); });
    // Part 2 test (13.txt) => 43663323
    run("13.txt", [](ifstream& fs) { aoc6(fs, true); });
    */

    // Problem set 7, test (14.txt) => 6440
    // run("14.txt", aoc7);
    // Part 1 (15.txt) => 249204891
    // Part 2 (15.txt) => 249666369
    // run("15.txt", aoc7);

    // Problem set 8, test (17.txt) => 6
    run("17.txt", [](ifstream& fs) { aoc8(fs, false); });
    // Part 1 (18.txt) => 16531
    run("18.txt", [](ifstream& fs) { aoc8(fs, false); });
    // Part 2 test (19.txt) => 6
    run("19.txt", [](ifstream& fs) { aoc8(fs, true); });
    // Part 2 (18.txt) => 24035773251517
    run("18.txt", [](ifstream& fs) { aoc8(fs, true); });
}
