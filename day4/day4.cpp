#include <fstream>
#include <set>
#include <map>
#include "personalized_utilities.h"
using namespace std;

int main() 
{
    ifstream file = ifstream("data.txt");
    string line = "";

    unordered_map<int, int> match_by_card = unordered_map<int,int>();
    int card_id = 1;

    while (getline(file, line)) {
        string data_card = string(find(line.begin(), line.end(), ':') + 2, line.end());
        auto win_numbers = unordered_set<int>();
        bool first = true;
        int match_count = 0;
        vector<string> numbers = split(move(data_card), '|');
        for (auto& sides : numbers) 
        {
            if (first) {
                for (int win_number  : g_fmt_view<int>(move(sides), ' ')) 
                    win_numbers.insert(win_number);
                first = false;
            }
            else {
                match_count = ranges::count_if(g_fmt_view<int>(move(sides), ' '), d_pred(is_into, win_numbers));
            }
        }
        match_by_card[card_id] = match_count;
        card_id++;
    }

    int card_count = match_by_card.size();
    auto cards_to_check = make_range(card_count); 
    int total_scratch_cards = 0;

    for (int j = 0; j < cards_to_check.size(); j++) 
    {
        total_scratch_cards++;
        int card_id_check = cards_to_check[j];
        int match_count = match_by_card[card_id_check];
        for (int i = 1; i <= match_count; i++) {
            cards_to_check.push_back(card_id_check + i);
        }
    }
    cout << total_scratch_cards << endl;
}