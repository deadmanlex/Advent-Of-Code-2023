#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;
using ul_int = unsigned long long int;

int main() 
{
    ifstream file = ifstream("data.txt");
    string line1 = "";
    string line2 = "";
    getline(file ,line1);
    getline(file, line2);
    vector<ul_int> times, distances;
    string str1 = string(find(line1.begin() + 1, line1.end(), ':') + 1, line1.end());
    string str2 = string(find(line2.begin() + 1, line2.end(), ':') + 1, line2.end());

    str1 = ReplaceAll(str1, " ", "");
    str2 =  ReplaceAll(str2, " ", "");

    times.push_back(stoull(str1));
    distances.push_back(stoull(str2));
    int answer = 0;
    vector<ul_int> possible_ways_to_beat;
    for (int i = 0 ; i < times.size(); ++i) {
       auto time = times[i];
       auto dist = distances[i];
       ul_int count = 0;
       for (ul_int i = 0; i < time; i++) {
            ul_int dist_done = i * (time - i);
            if (dist_done > dist) {
                count++;
            }
       }
       possible_ways_to_beat.push_back(count);
    }
    answer= multi_all(possible_ways_to_beat);
    cout << answer << endl;
}