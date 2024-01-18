#include <fstream>
#include <set>
#include <map>
#include <Eigen/Dense>
#include "personalized_utilities.h"
using namespace std;

using map_t = vector<tuple<pair<unsigned int,unsigned int>,unsigned int>>;
using ul_int = unsigned long long int;

enum Orient {North, South, West, East};

unordered_map<Orient, pair<int,int>> directions = {
    {North, {-1,0}}, 
    {South, {1, 0}}, 
    {West,  {0,-1}}, 
    {East,  {0, 1}}, 
};

unordered_map<Orient, Orient> inverse_direct = {
    {North, South}, 
    {South, North}, 
    {West,  East}, 
    {East,  West}, 
};

vector<pair<int,int>> loop_coords = {};

constexpr Orient to_orient(pair<int,int>& old_coord, pair<int,int>& next_coord) {
    auto [x,y] = next_coord;
    auto [ox,oy] = old_coord;
    auto nx = x - ox; 
    auto ny = y - oy;
    if (nx == 0 && y == 1) return South;
    else if (nx == 1 && y == 0) return East;
    else if (nx == -1 && y == 0) return West;
    else if (nx == 0 && y == -1) return North;
}

float calculate_inside_points(const vector<pair<int,int>> & boundary_points) {
    float area = 0;
    for (auto it = boundary_points.begin(); next(it) != boundary_points.end(); it++ ) {
        auto next_it = next(it);
        auto [y1,x1] = *it;
        auto [y2,x2] = *next_it;
        auto matrix = Eigen::Matrix2f();
        matrix << x1,x2,y1,y2;
        auto det = matrix.determinant();
        area += matrix.determinant();
    }
    return area/2.f - (boundary_points.size()-1.f)/2.f + 1;
}

int search_point(pair<int,int> path1, Orient lastOrient1, pair<int,int> start_point, const matrix<char>& grid, ul_int it_count) {
    while (!(path1.first == start_point.first && path1.second == start_point.second))
    {
    loop_coords.push_back(path1);
    pair<int,int> next_point1, next_point2; 
    auto next_point_and_orient = [&](auto last1, Orient source_orient) { 
        auto [x,y] = last1;
        Orient next_orient;
        if (grid[x][y] == '|') {
             next_orient = (source_orient == South) ? North : South;
        }
        else if (grid[x][y] == '-') {
            next_orient = (source_orient == West) ? East : West;
        }
        else if (grid[x][y] == '7') {
            next_orient = (source_orient == South) ? West : South;
        }
        else if (grid[x][y] == 'J') {
            next_orient = (source_orient == North) ? West : North;
        }
        else if (grid[x][y] == 'F') {
            next_orient = (source_orient == South) ? East : South;
        }
        else if (grid[x][y] == 'L') {
            next_orient = (source_orient == North) ? East : North;
        }
        auto [nx, ny] = directions[next_orient];
        return make_tuple(make_pair(x+nx, y+ny), inverse_direct[next_orient]);
    };
    auto [npath1, nOrient1] = next_point_and_orient(path1, lastOrient1);
    path1 = npath1;
    lastOrient1 = nOrient1;
    it_count++;
    }
    loop_coords.push_back(path1);
    return it_count;   
}

int main() 
{
    ifstream file = ifstream("data.txt");
    matrix<char> pipe_grid = matrix<char>();
    string line = "";
    int answer1 = 0, answer2 = 0;
    int x_start_pos = 0;
    int y_start_pos = 0;
    bool s_found = false;
    while (getline(file, line)) { 
        vector<char> char_entries = vector<char>();
        if (!s_found) x_start_pos = 0;

        for (auto c : line) { 
            char_entries.push_back(c); 
            if (c == 'S') {
                s_found = true;
            }
            else if (!s_found) {
                x_start_pos++;
            }
        }
        pipe_grid.push_back(char_entries);
        if(!s_found) {
            y_start_pos++;
        }
    }
    loop_coords.push_back(make_pair(y_start_pos, x_start_pos));
    auto [nx,  ny] = directions[South]; 
    answer1 = search_point(make_pair(y_start_pos + nx,x_start_pos + ny), inverse_direct[South], make_pair(y_start_pos, x_start_pos), pipe_grid,1);
    cout << answer1 / 2 << endl;
 
    cout << calculate_inside_points(loop_coords) << endl;
}