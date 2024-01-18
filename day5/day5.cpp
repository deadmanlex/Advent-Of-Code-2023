#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;

constexpr unsigned int map_to_data(const map_t&& data, unsigned dt) {
    auto it = find_if(data.begin(), data.end(), [&](auto it) {
        auto & [s, _] = it;
        auto & [bmin, bmax] = s;
        return (int32_t)(((uint32_t)dt - (uint32_t)bmin) | ((uint32_t)bmax - (uint32_t)dt)) >= 0;
    });
    if (it == data.end()) {
        return dt;
    }
    auto & [source, dest] = *it;
    auto [bmin ,_] = source; 
    return dest + (dt - bmin);
}

int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";
    int somme = 0;
    bool first = true;
    vector<unsigned long long int> seeds = vector<unsigned long long int>();
    set<unsigned long long int> dest = set<unsigned long long int>();
    map_t seed_soil, soil_fertilizer,fert_water, water_light, light_temp, temp_humidity, humity_location; 
    auto extract_map_data = [&](map_t& map, ifstream& file) {
        getline(file , line);
        while (line != "") {
            auto view = g_fmt_view<unsigned long long int>(move(line), ' ');
            vector<unsigned long long int> et = vector<unsigned long long int>{view.begin(), view.end()};
            auto dest_start = et[0];
            auto source_start = et[1];
            auto range_l = et[2];
            map.push_back(make_tuple(make_pair(source_start, source_start+range_l-1), dest_start));
            getline(file, line);
        }
    };

    while (getline(file, line)) {
        if (first) {
            first=false;
            auto data_str = string(find(line.begin(), line.end(), ':'), line.end());
            for (auto dt: g_fmt_view<unsigned long long int>(move(data_str), ' '))
            { 
                seeds.push_back(dt);
            }
        }
        if (line.find("seed-to-soil") != string::npos){
            extract_map_data(seed_soil, file);
        }
        if (line.find("soil-to-fertilizer") !=string::npos){
            extract_map_data(soil_fertilizer, file);
        }
        if (line.find("fertilizer-to-water")!= string::npos) {
            extract_map_data(fert_water, file); 
        }
        if (line.find("water-to-light")!= string::npos) {
            extract_map_data(water_light, file); 
        }
        if (line.find("light-to-temperature")!= string::npos) {
            extract_map_data(light_temp, file); 
        }
        if (line.find("temperature-to-humidity")!= string::npos) {
            extract_map_data(temp_humidity, file); 
        }
        if (line.find("humidity-to-location")!= string::npos) {
            extract_map_data(humity_location, file); 
        }
    }
    unsigned long long int min = 100000000000000;
    for (auto it = seeds.begin(); it != seeds.end(); it = std::next(it, 2)) {
        auto start = *it;
        auto range = *(next(it));
        for (unsigned long long int i = start; i < start+range; ++i) {
            auto dest = fold_r_apply(map_to_data, i, seed_soil,soil_fertilizer, fert_water, water_light, light_temp, temp_humidity, humity_location);       
            min = (min > dest) ? dest : min;
    }
    cout << min << endl;
}
}