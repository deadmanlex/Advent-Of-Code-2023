#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using ul_int = unsigned long long int;
using coords = pair<ul_int, ul_int>;

enum States {Working = '.', Broken = '#', Unknown = '?'}; 

std::unordered_map<tuple<string,string>, ul_int, HashTuple> arrangement_cache = std::unordered_map<tuple<string,string>, ul_int, HashTuple>();
template<class It, class It2>
ul_int memo_count_arrangements(It state_str, It end_str, It2 n_it, It2 end_n_it) {
    string st1 = "", n_str = "";
    for_each(state_str, end_str, [&](auto c){ st1 += static_cast<char>(c); });
    for_each(n_it, end_n_it, [&](auto c){ n_str += to_string(c); });
    auto tuple = make_tuple(st1, n_str);
    if (is_into(tuple, arrangement_cache)) {
        return arrangement_cache[tuple];
    }
    else {
        auto results = count_arrangements( state_str,  end_str,  n_it,  end_n_it);
        arrangement_cache[tuple] = results;
        return results;
    } 
}

template<class It, class It2>
ul_int count_arrangements(It state_str, It end_str, It2 n_it, It2 end_n_it) {
    if (state_str == end_str && n_it == end_n_it) 
        return 1;
    else if (state_str == end_str && n_it != end_n_it) 
        return 0;
    auto working_spring = [&](auto c) { return c == Working; };
    auto unknown_spring = [&](auto c) { return c == Unknown; };
    switch (*state_str) {
        case Working: 
            return memo_count_arrangements(next(state_str), end_str, n_it, end_n_it);
        break;
        case Broken: 
        {
            if (n_it == end_n_it) 
                return 0;
            auto n_working = find_if(state_str, end_str, working_spring);
            auto n_unknown = find_if(state_str, end_str, unknown_spring);
            if (n_working < n_unknown || (n_working == end_str && n_unknown == end_str)) {
                return memo_count_arrangements(n_working, end_str, (distance(state_str, n_working) == *n_it) ? next(n_it) : n_it, end_n_it);
            }
            else {
                int expected = *n_it;
                auto dist = distance(state_str, n_unknown);
                if (distance(state_str, n_working) == expected) {
                    return memo_count_arrangements(n_working, end_str, next(n_it), end_n_it);
                }
                else if (dist == expected) {
                    *n_unknown = Working;
                    auto result = memo_count_arrangements(n_unknown, end_str, next(n_it), end_n_it);
                    *n_unknown = Unknown;
                    return result;
                }
                else {
                    int missing = expected - dist; 
                    if (missing < 0) {
                        return 0;
                    }
                    *n_it -= dist;
                    *n_unknown = Broken;
                    ul_int n1 = memo_count_arrangements(n_unknown, end_str, n_it, end_n_it);
                    *n_it += dist;
                    *n_unknown = Working;
                    ul_int n2 = memo_count_arrangements(n_unknown, end_str, n_it, end_n_it);
                    *n_unknown = Unknown;
                    return n1 + n2;
                }
            }
          
        }
        break;
        case Unknown: 
            *state_str = Working; 
            int n1 = memo_count_arrangements(state_str, end_str, n_it, end_n_it);
            *state_str = Broken; 
            int n2 = memo_count_arrangements(state_str, end_str, n_it, end_n_it);
            *state_str = Unknown;
            return n1 + n2;
        break;
    }
}

vector<States> parse_states(const string& state_str) {
    auto states = vector<States>();
    states.reserve(state_str.size());
    for (auto c : state_str) {
        switch (c) {
            case '?': states.push_back(Unknown);
            break;
            case '#': states.push_back(Broken);
            break;
            case '.': states.push_back(Working);
            break;
        }
    }
    return states;
}


ul_int count_arrangements(const string& state_str, vector<int>& numbers) {
    vector<States> states = parse_states(state_str); 
    return memo_count_arrangements(states.begin(), states.end(), numbers.begin(), numbers.end());
}


int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";
    ul_int answer = 0, answer2 = 0;
    vector<char> elems = {'.', '#'};
    vector<vector<string>> config_by_lengths; 
    config_by_lengths.reserve(18);

    for (int i = 0; i < 20; ++i) {
        vector<string> config = generate_combinations(elems, i + 1);
        config_by_lengths.push_back(config);
    }
    
    while (getline(file, line)) {
        vector<string> splitted = split(move(line), " ");
        auto numbers = g_fmt_view<int>(move(splitted[1]), ',');
        auto v_n = vector<int>();
        for (auto n : numbers) v_n.push_back(n);
        answer += count_arrangements(splitted[0], v_n);
    } 
    cout << answer << endl;
}