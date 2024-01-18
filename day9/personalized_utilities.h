#pragma once
#include <iostream> 
#include <vector> 
#include <memory>
#include <string> 
#include <iomanip>
#include <array>
#include <ranges> 
#include <utility>
#include <charconv>
#include <regex> 
#include <fstream>
#include <numeric>
#include <string_view>
#include <algorithm> 
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>


template <typename T>
using matrix = std::vector<std::vector<T>>;

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

#define NUMBER_RANGE 1000000
#define m_pred(_pred_FN) [&](auto& value) { return _pred_FN(value); }  
#define d_pred(_pred_FN, c_value) [&](auto value) { return _pred_FN(value, c_value); }  

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

constexpr std::vector<int> make_range(int n, int initial_value=1) {
    std::vector<int> n_list = std::vector<int>(n);
    std::iota(n_list.begin(), n_list.end(), 1);
    return n_list;
}

template<class Map_Container>
constexpr auto get_max_value(const Map_Container&& map) -> Map_Container::mapped_type {
    return max_element(map.begin(), map.end(), [](auto& a, auto&b){ return a.second < b.second; })->second;
}

template<size_t N>
constexpr std::array<bool, N+1> make_erathostene_sieve() {
    using namespace std;
    std::array<bool, N+1> sieve = std::array<bool, N+1>();
    sieve.fill(true);
    for (int i = 2; i < (N + 1)/2; i++) {
        for (int j = 2*i; j < N+1; j += i) 
            sieve[j] = false;
    }
    return sieve;
}

template<class set_type, class T> 
constexpr bool is_into(T value, const set_type& set) {
    return set.find(value) != std::end(set);
}

template<class T>
constexpr bool in_bounds(T value, T min , T max) {
    return (int32_t)(((uint32_t)value - (uint32_t)min) | ((uint32_t)max - (uint32_t)value)) >= 0;
}

// trim from both ends (in place)
constexpr void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}


constexpr auto split(const std::string&& str, char delimiter) -> std::vector<std::string> {
    auto to_string_l = [](auto&& r) -> std::string {
        const auto data = &*r.begin();
        const auto size = static_cast<std::size_t>(std::ranges::distance(r));
        return std::string{data, size};
    };
    auto range = str | 
                 std::ranges::views::split(delimiter) | 
                 std::ranges::views::transform(to_string_l);
    return {std::ranges::begin(range), std::ranges::end(range)};
}


constexpr auto split(const std::vector<std::string>&& str_list, char delimiter) -> std::vector<std::string> {
    auto to_splitted_string = [delimiter](auto&& r) -> std::vector<std::string> { return split(std::move(r) , delimiter); };
    auto range = str_list | 
                std::ranges::views::transform(to_splitted_string) | 
                std::ranges::views::join;
    std::vector<std::ranges::range_value_t<decltype(range)>> v;
    std::ranges::move(range, std::back_inserter(v));
    return v;
}

constexpr auto split(const std::string&& str, const std::string&& delimiter) -> std::vector<std::string> {
    auto to_string_l = [](auto&& r) -> std::string {
        const auto data = &*r.begin();
        const auto size = static_cast<std::size_t>(std::ranges::distance(r));
        return std::string{data, size};
    };
    auto range = str | 
                 std::ranges::views::split(delimiter) | 
                 std::ranges::views::transform(to_string_l);
    return {std::ranges::begin(range), std::ranges::end(range)};
}


constexpr auto split(const std::vector<std::string>&& str_list, const std::string&& delimiter) -> std::vector<std::string> {
    auto to_splitted_string = [delimiter](auto&& r) -> std::vector<std::string> { return split(std::move(r) , move(delimiter)); };
    auto range = str_list | 
                std::ranges::views::transform(to_splitted_string) | 
                std::ranges::views::join;
    std::vector<std::ranges::range_value_t<decltype(range)>> v;
    std::ranges::move(range, std::back_inserter(v));
    return v;
}

template<typename... Args>
constexpr auto split(const std::string&& str, Args... delimiters) -> std::vector<std::string> {
    std::vector<std::string> spl = std::vector<std::string>{ str };
    ([&]{ spl = split(std::forward<std::vector<std::string>>(spl), delimiters); } (), ...);
    return spl;
}

template <typename>
struct is_pair : std::false_type
{ };

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{ };

template <typename T, typename U>
struct is_pair<std::pair<T const, U>&> : std::true_type
{ };

template<class T>
constexpr auto multi_all(T& l)  { 
    int multiply = 1;
    auto it = std::begin(l);
    std::for_each(it, std::end(l), [&](auto i) {
        if constexpr (is_pair<decltype(it)>()) multiply *= i.second;
        else if constexpr (!is_pair<decltype(it)>()) multiply *= i;
    });
    return multiply;
}

std::vector<int> make_prime_divisors_table(int size) {
    using namespace std;
    vector<int> table = vector<int>(size,0);
    for (int i = 2; i < (size + 1)/2; i++) {
        for (int j = 2*i; j < size+1; j += i)
           if (table[j] == 0) table[j] = i;
    }
    for (int i = 0; i < table.size(); ++i) {
        if (table[i] == 0) table[i] = i;
    }
    return table;
}

int count_distinct_primes(int n, const std::vector<int>& prime_divisors_table) {
    std::unordered_set<int> distinct_prime_factors = std::unordered_set<int>();
    int i = n;
    while (i > 1) {
        if (!is_into(prime_divisors_table[i], distinct_prime_factors)) 
            distinct_prime_factors.insert(prime_divisors_table[i]); 
        i /= prime_divisors_table[i];
    }
    return distinct_prime_factors.size();
}

template<class T, class Fn>
constexpr void apply_on_nn(matrix<T>&& mat, int i, int j, Fn proc) {
    std::set<std::pair<int,int>> nearest_neighbors = {
        {i + 1, j},
        {i, j + 1},
        {i - 1, j},
        {i, j - 1},
        {i + 1, j + 1},
        {i - 1, j + 1},
        {i + 1, j - 1},
        {i - 1, j - 1}
    }; 
    for (const std::pair<int,int> & neighbor : nearest_neighbors) {
        auto & [n_i, n_j] = neighbor;
        if (!in_bounds(n_i, 0, mat.size()) || !in_bounds(n_j, 0, mat[0].size()) ) 
            continue;
        proc(n_i, n_j);
    }
};

template<class T>
constexpr auto g_fmt_view(const std::string&& source, const char delimiter) { 
    return source | std::ranges::views::split(delimiter) | 
    std::ranges::views::filter([](auto v) { 
        T i = 0;
        auto [_, ec] = std::from_chars(v.data(), v.data() + v.size(), i);
        return (ec != std::errc::invalid_argument);
    }) |
    std::ranges::views::transform([](auto v) {
        T i = 0;
        std::from_chars(v.data(), v.data() + v.size(), i);
        return i;
    });
}

template<class T, class Fn, class... Args>
constexpr auto fold_r_apply(Fn op, T  init, Args&&...args) {
    ([&]{ init = op(std::move(args), init); } (), ...);
    return init;
}

template<class T, class Fn, class... Args>
constexpr auto fold_l_apply(Fn op, T  init, Args&&...args) {
    (..., [&]{ init = op(std::move(args), init); } ());
    return init;
}

template<size_t N>
std::vector<std::vector<int>> read_matrix_file(const std::string& file_name, const char delimiter = ' ') 
{
    using std::ranges::for_each;
    std::ifstream file_stream(file_name);
    if (!file_stream.is_open()) {
        return std::vector<vector<int>>();
    }
    std::vector<vector<int>> new_matrix(N);
    std::string buffer_read = "";

    int current_row = 0;
    while (getline(file_stream, buffer_read)) {
        new_matrix[current_row].reserve(N);

        for_each(g_fmt_view<int>(std::move(buffer_read), ','), [&](auto const& new_entry) { 
            new_matrix[current_row].push_back(new_entry); 
        });
        current_row++;
    }
    return new_matrix;
}

constexpr int HCD(int n, int d) {
    if (d==0) { return n; }
    return HCD(d, n % d);
}

