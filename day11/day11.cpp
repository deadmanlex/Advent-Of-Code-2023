#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;
using ul_int = long long int;
using coords = pair<ul_int,ul_int>;
matrix<char> galaxy_map; 
int galaxy_distance = 2;

vector<coords> expand_map() 
{
    vector<coords> galaxy_coords;
    vector<int> empty_rows = vector<int>(galaxy_map.size(), 0);
    vector<int> empty_columns = vector<int>(galaxy_map.size(), 0);
    auto new_galaxy_row = vector<char>(galaxy_map[0].size(), '.');
    
    for (int i = 0; i < galaxy_map.size(); ++i) 
    {
        auto galaxy_row = galaxy_map[i];
        if (find(galaxy_row.begin(), galaxy_row.end(), '#') == galaxy_row.end()) {
            empty_rows[i] = 1;
        }
    }
    for (int i = 0; i < galaxy_map[0].size(); ++i) {
        bool col_empty = true;
        for (int j = 0; j < galaxy_map.size(); ++j) {
            if (galaxy_map[j][i] == '#') {
                col_empty = false;
                break;
            }
        }
        if (col_empty) {
            empty_columns[i] = 1;
        }
    } 

    ul_int galaxy_row = 0;
    for (int i = 0; i < galaxy_map.size(); ++i) {
       ul_int galaxy_col = 0;
        for (int j = 0; j < galaxy_map[0].size(); ++j) {
            if (galaxy_map[i][j] == '#') { 
                galaxy_coords.push_back(make_pair(galaxy_row, galaxy_col));
            }
            galaxy_col += (empty_columns[j] == 1) ? galaxy_distance : 1;
        }
        galaxy_row += (empty_rows[i] == 1) ? galaxy_distance : 1;

    }
    return galaxy_coords; 
}

int main() 
{
    ifstream file = ifstream("data.txt");
    string line;
    ul_int answer = 0, answer2 = 0;
    while (getline(file, line)) { 
        vector<char> char_entries = vector<char>();
        for (auto c : line) { 
            char_entries.push_back(c); 
        }
        galaxy_map.push_back(char_entries);
    }
    vector<coords> galaxy_coords = expand_map();
    ul_int min = 10000000;
    for (int i = 0; i < galaxy_coords.size(); ++i) { 
        for (int j = i + 1; j < galaxy_coords.size(); ++j) {
            auto [x1,y1] = galaxy_coords[i]; 
            auto [x2,y2] = galaxy_coords[j];
            auto distance = abs(x1 - x2) + abs(y1 - y2);
            answer += distance;
        }
    }
    cout << answer << endl;
    galaxy_distance = 1000000;
    galaxy_coords = expand_map();
    min = 1000000000;
    for (int i = 0; i < galaxy_coords.size(); ++i) { 
        for (int j = i + 1; j < galaxy_coords.size(); ++j) {
            auto [x1,y1] = galaxy_coords[i]; 
            auto [x2,y2] = galaxy_coords[j];
            auto distance = abs(x1 - x2) + abs(y1 - y2);
            answer2 += distance;
        }
    }
    cout << answer2 << endl;
}