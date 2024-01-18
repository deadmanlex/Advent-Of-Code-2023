
#include <fstream>
#include <map>
#include "personalized_utilities.h"
using namespace std;


int main() 
{
    ifstream file = ifstream("data.txt");
    string line_it = "";
    int curr_id = 1;
    int somme = 0;
    while (getline(file,line_it)) {
        const string& removed_set = line_it.substr(line_it.find(":") + 1);
        unordered_map<string, int> cubes = {
            {"blue", 0},
            {"red", 0},
            {"green", 0}
        };
        for (auto str : split(move(removed_set), ';', ',')) {
            int count = stoi(string(&str[1]));
            string key = string(std::find_if(str.begin(), str.end(), m_pred(isalpha)), str.end());
            cubes[key] = max(cubes[key], count);
        }
        somme += multiply_all(cubes);
    }
    cout << somme << endl;
}