#include <iostream>
#include <string>
#include <unordered_set> 
#include <algorithm>
#include <type_traits>
#include <fstream>
#include <set>
#include <functional>   
#include <list>
#include <memory>
#include <chrono>
#include <regex>
#include <ctime>
#include "personalized_utilities.h"
using namespace std;

unordered_map<string, string> string_numbers = unordered_map<string,string>{ 
    {"one", "1"}, 
    {"two", "2"}, 
    {"three", "3"},
    {"four", "4"}, 
    {"five", "5"}, 
    {"six", "6"},
    {"seven", "7"}, 
    {"eight", "8"}, 
    {"nine", "9"} 
};

int main() 
{
    ifstream data = ifstream("day1.txt");
    string line_it = string("");
    int sum = 0;
    
    while (getline(data, line_it)) {
        string number = "";
        for (auto it = line_it.begin(); it != line_it.end(); it++) {
            if (isdigit(*it)){
                number += *it;
                break;
            }
            else if (it + 2 != line_it.end()) {
                string sub_str = string(it, it + 3);
                if (is_into(sub_str, string_numbers)) {
                    number += string_numbers[sub_str];
                    break;
                }
                else if (it + 3 != line_it.end()) {
                    sub_str += *(it + 3);
                    if (is_into(sub_str, string_numbers)) {
                        number += string_numbers[sub_str];
                        break;
                    }
                    else if (it + 4 != line_it.end()) {
                        sub_str += *(it + 4);
                        if (is_into(sub_str, string_numbers)) {
                            number += string_numbers[sub_str];
                            break;
                        }
                    }
                }
            }
        }
        
         for (auto it = line_it.rbegin(); it != line_it.rend(); it++) {
            if (isdigit(*it)){
                number += *it;
                break;
            }
            else if (it - 2 != line_it.rend()) {
                string sub_str = ""; 
                sub_str += *it;
                sub_str += *(it - 1);
                sub_str += *(it - 2);
                if (is_into(sub_str,string_numbers)) {
                    number += string_numbers[sub_str];
                    break;
                }
                else if (it - 3 != line_it.rend()) {
                    sub_str += *(it - 3);
                    if (is_into(sub_str,string_numbers)) {
                        number += string_numbers[sub_str];
                        break;
                    }
                    else if (it - 4 != line_it.rend()) {
                        sub_str += *(it - 4);
                        if (is_into(sub_str,string_numbers)) {
                            number += string_numbers[sub_str];
                            break;
                        }
                    }
                }
            }
        }
        sum += stoi(number);
    }
    cout << sum << endl;
}