#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;
using ul_int = unsigned long long int;

int extrapolate_sequence(const std::vector<int>& og_seq) {
    auto it = og_seq.begin();
    auto it_end = og_seq.end();
    bool all_0 = false;
    auto get_diff = [&](auto it_be, auto it_end) {
        all_0 = true;
        std::vector<int> diff = {};
        while (std::next(it) != it_end) {
            auto dif = *(next(it)) - *it;
            if (dif != 0) all_0 = false;
            diff.push_back(dif);
            it++;
        }
        return diff;
    };
    vector<vector<int>> diff_seqs;
    vector<int> diff_seq;
    diff_seqs.push_back(og_seq);
    while (!all_0) {
        diff_seq = get_diff(it, it_end);
        it = diff_seq.begin();
        it_end = diff_seq.end();
        diff_seqs.push_back(diff_seq);
    }
    int next_number = 0;
    for (auto r_it = diff_seqs.rbegin(); r_it != diff_seqs.rend(); r_it++) {
        next_number = *r_it->begin() - next_number;
    }
    return next_number;
}

int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";
    int answer = 0;
    while (getline(file, line)) { 
        vector<int> li = {};
        for (auto it : g_fmt_view<int>(move(line), ' ')) {
            li.push_back(it);
        }
        answer+= extrapolate_sequence(li);
    }

    cout << answer << endl;
}