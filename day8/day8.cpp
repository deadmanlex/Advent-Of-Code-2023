#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;
using ul_int = uint64_t;

bool end_with_z(string& sz) { return sz[2] != 'Z'; }

int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";
    auto hand_list = std::vector<std::pair<string, ul_int>>();
    unordered_map<string, pair<string,string>> node_map = unordered_map<string, pair<string,string>>();
    ul_int answer = 0;
    bool first = true;
    string directions = "";
    vector<string> start_nodes = {};

    clock_t start_point = clock();
    while (getline(file, line)) {
        if (first) {
            directions = line;
            first = false;
            continue;
        }
        if (line == "") continue;
        auto spl = split(move(line), move(" = "));
        auto key = spl[0];
        auto splitted_pair = split(move(ReplaceAll(ReplaceAll(spl[1], "(", ""), ")", "")), move(", ")); 
        node_map[key] = make_pair(splitted_pair[0], splitted_pair[1]);
        if (key[2] == 'A') start_nodes.push_back(key);
    }
    vector<const string*> nodes = {}; 

    for_each(start_nodes.begin(), start_nodes.end(), [&](const string & node) {
        nodes.push_back(&node);
    });
    vector<ul_int> entries = {};
    for (auto node : nodes) {
        auto dir_it = directions.begin();
        int somme = 0;
        while (node->at(2) != 'Z') {
            if (dir_it == directions.end()) dir_it = directions.begin();
            const auto & [left, right] = node_map[*node];
            node = (*dir_it == 'R') ? &right : &left;
            dir_it++;
            somme++;
        }
        entries.push_back(somme);
    }
    answer = entries[0];
    auto lcm = [](ul_int n1,ul_int n2) { return n2/HCD(n1,n2) * n1; };
    cout << fold_l_apply(lcm, entries[0], entries[1], entries[2],entries[3],entries[4],entries[5]) << endl;
    cout << "Speed : " << (double)(clock() - start_point)/CLOCKS_PER_SEC*1000000.0 << "ms" << endl;
}