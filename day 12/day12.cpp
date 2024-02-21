#include <fstream>
#include <set>
#include <map>
#include <regex> 
#include <format>
#include "personalized_utilities.h"
using namespace std;

using ul_int = unsigned long long int;
using coords = pair<ul_int, ul_int>;

enum States {Working = '.', Broken = '#', Unknown = '?'}; 

std::unordered_map<tuple<string,string>, ul_int, HashTuple> arrangement_cache = std::unordered_map<tuple<string,string>, ul_int, HashTuple>();

template<class It, class It2>
ul_int memo_count_arrangements(It state_str, It end_str, It2 n_it, It2 end_n_it);

const regex is_working = regex(R"(^(\.).*$)");
const regex is_broken_unknown =  regex(R"(^#+\?+.*$)"); 
const regex is_unknown = regex(R"(^\?+.*$)");

template<class It, class It2>
ul_int count_arrangements(It state_str, It end_str, It2 n_it, It2 end_n_it) {
    if (state_str != end_str && n_it == end_n_it && *state_str == '#') {
        return 0;
    }
    if (std::regex_match(state_str, end_str, is_working)) {
        return memo_count_arrangements(next(state_str), end_str, n_it, end_n_it); 
    }
    else if (n_it != end_n_it && std::regex_match(state_str, end_str, regex("^#{" + to_string(*n_it) + "}\\?+.*$"))) {
        auto next_unknown = std::find(state_str, end_str, '?'); 
        return memo_count_arrangements(std::next(next_unknown), end_str, std::next(n_it), end_n_it); 
    }
    else if (n_it != end_n_it && std::regex_match(state_str, end_str, regex("^#{" + to_string(*n_it) + "}(([^\\?#]+.*)|$)$"))) {
        return memo_count_arrangements(std::find(state_str, end_str, '.'), end_str, std::next(n_it), end_n_it); 
    }
    else if (n_it != end_n_it && std::regex_match(state_str, end_str, is_broken_unknown)) {
        auto curr_n = state_str;
        ul_int n_broken = 0;
        ul_int n_unknown = 0;
        while (*curr_n == '#') {
            curr_n++; 
            n_broken++;
        }
        while (*curr_n == '?') {
            curr_n++; 
            n_unknown++;
        } 
        ul_int missing = min(*n_it - n_broken, n_unknown); 
        if (missing >= 0) {
            string tmp_str = string(state_str, end_str);
            std::replace(tmp_str.begin(), tmp_str.begin() + n_broken + missing, '?', '#');
            return memo_count_arrangements(tmp_str.begin(), tmp_str.end(), n_it, end_n_it); 
        }
        else return 0;
    }
    else if (std::regex_match(state_str, end_str, is_unknown)) {
        *state_str = '.';
        ul_int FstResult = memo_count_arrangements(state_str, end_str, n_it, end_n_it);
        *state_str = '#';
        ul_int SndResult = memo_count_arrangements(state_str, end_str, n_it, end_n_it);
        *state_str = '?';
        return FstResult + SndResult;
    }
    else if (state_str == end_str && n_it == end_n_it) 
        return 1;
    else {
        return 0;
    }
}


ul_int count_arrangements(const string& state_str, vector<int>& numbers) {
    string tmp_str = string(state_str);
    return memo_count_arrangements(tmp_str.begin(), tmp_str.end(), numbers.begin(), numbers.end());
}



int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";
    ul_int answer = 0, answer2 = 0;

    while (getline(file, line)) {
        vector<string> splitted = split(move(line), " ");
        auto numbers = g_fmt_view<int>(move(splitted[1]), ',');
        auto v_n = vector<int>();
        for (auto n : numbers) v_n.push_back(n);
        answer += count_arrangements(splitted[0], v_n);
        
        string split_fold = splitted[0];
        auto numbers_fold = v_n;
        for (int i = 0; i < 4; i++){
            split_fold += "?" + splitted[0];
            for (auto n : v_n) numbers_fold.push_back(n);
        } 
        answer2 += count_arrangements(split_fold, numbers_fold);
    } 
    cout << answer << endl;
    cout << answer2 << endl; 
}



template<class It, class It2>
ul_int memo_count_arrangements(It state_str, It end_str, It2 n_it, It2 end_n_it) {
    string st1 = "", n_str = "";
    for_each(state_str, end_str, [&](auto c){ st1 += static_cast<char>(c); });
    if(n_it != end_n_it) for_each(n_it, end_n_it, [&](auto c){ n_str += to_string(c) + ","; });
    auto tuple = make_tuple(st1, n_str);
    if (is_into(tuple, arrangement_cache)) {
        return arrangement_cache[tuple];
    }
    else {
        ul_int results = count_arrangements( state_str,  end_str,  n_it,  end_n_it);
        arrangement_cache[tuple] = results;
        return results;
    } 
}