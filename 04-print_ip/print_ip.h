
#ifndef PRINT_IP_H
#define PRINT_IP_H

#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <ranges>
#include <concepts>
/** @file */
template <typename T> requires(std::integral<T>) // Печать побайтно целых
std::string to_string(T t) {
    std::vector<std::string> addr = {};

    for (auto i = sizeof (t); i > 0; --i) {
        addr.emplace_back(std::to_string(static_cast<int16_t>(t & 0xFF)));
        t /= 256;
        addr.emplace_back(".");
    }
    addr.pop_back();

    std::string result;
    for(const auto& element: std::ranges::reverse_view{addr})
        result.append(element);

    return result;
}

template <typename T> requires(std::is_convertible_v<T, std::string_view>) // Печать строкоподобных
std::string to_string(T t) {
    return std::string{t};
}

/**
 * @brief Check T is std::vector
 * @tparam T template parameter
 */
template<typename T>
concept is_vector = std::same_as<std::decay_t<T>, std::vector<typename std::decay_t<T>::value_type> >;

/**
 * @brief Check T is std::list
 * @tparam T template parameter
 */
template<typename T>
concept is_list = std::same_as<std::decay_t<T>, std::list<typename std::decay_t<T>::value_type> >;

template <typename T> requires(is_vector<T> || is_list<T>)  // Печать std::vector+std::list
std::string to_string(T t) {
    size_t i{1};

    std::string result;
    for(const auto& element: t) {
        result.append(std::to_string(element));
        if (i++ != t.size()) result.append(".");
    }
    return result;
}

template<typename H, typename... T>
struct is_tuple_of_same_types_ {
    static constexpr bool value = false;
};

template<typename Head>
struct is_tuple_of_same_types_<Head, std::tuple<>> {
    static constexpr bool value = true;
};

template<typename A, typename Head, typename... Tail>
struct is_tuple_of_same_types_<A, std::tuple<Head, Tail...>> {
static constexpr bool value = std::is_same_v<A, Head> && is_tuple_of_same_types_<A, std::tuple<Tail...>>::value;
};

template<typename T>
struct is_tuple_of_same_types {
    static constexpr bool value = false;
};

template<typename H, typename... T>
struct is_tuple_of_same_types<std::tuple<H, T...>> {
static constexpr bool value = is_tuple_of_same_types_<H, std::tuple<H, T...>>::value;
};

template<>
struct is_tuple_of_same_types<std::tuple<>> {  // На случай пустого std::tuple (пусть тоже корректно обрабатывается)
    static constexpr bool value = true;
};

template<size_t i, typename... T>
std::string tuple_print(const std::tuple<T...>& t) {
    std::string result;
    result.append(std::to_string(get<i>(t)));

    if constexpr((i+1) != std::tuple_size_v<std::tuple<T...>>)
        result.append(".");

    if constexpr((i+1) < std::tuple_size_v<std::tuple<T...>>)
        result.append(tuple_print<i+1>(t));

    return result;
}

/**
 * @brief Check T is std::tuple with one type elements
 * @tparam T template parameter
 */
template<typename T>
concept is_tup = is_tuple_of_same_types<T>::value;

template<typename T>
std::string to_string(T t) requires is_tup<T> {  // Печать std::tuple c элементами одного типа
    if constexpr (std::tuple_size_v<T> != 0) { // компиляция не должна падать при пустом std::tuple
        return tuple_print<0>(t);
    }
    return std::string{""};
}


/**
 * @brief Print ip to std::cout
 * @tparam T template parameter, should be integer type or std::string/vector/list/tuple from the same type elements
 * @param t printing value
 * **Example using**
 * @code
 * print_ip( std::make_tuple(123, 456, 789, 0, 222));
 * @endcode
 */
template<typename T>
void print_ip(T t) {  // Обернул вызов to_string - чтобы делать модульное тестирование над ним
    std::cout << to_string(t) << "\n";
}

#endif
