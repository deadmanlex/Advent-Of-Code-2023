
#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;


matrix<char> read_grid_file()
{
    ifstream file = ifstream("data.txt");
    string line_iterator = "";
    matrix<char> grid = matrix<char>();

    while (getline(file, line_iterator)) {
        std::vector<char> line = vector<char>();
        for (auto entry : line_iterator) 
            line.push_back(entry ); 
        grid.push_back(line);
    }
    return grid;
}

int main() 
{
    matrix<char> grid = read_grid_file();
   
    cout << somme << endl;
}