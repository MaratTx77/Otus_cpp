#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

int main() {
    std::vector<boost::asio::ip::address_v4> ip_pool;

    try {
        for (std::string line; std::getline(std::cin, line);) { // Читаем адреса как беззнаковые 4 байтные числа
            std::vector<std::string> result;
            boost::split(result, line, boost::is_any_of("\t"));
            ip_pool.push_back(boost::asio::ip::address_v4::from_string(result.at(0)));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<>()); // Обратная сортировка

        for(const auto& addr: ip_pool) // Печать
            std::cout << boost::asio::ip::address_v4(addr) << std::endl;

        for(const auto& addr: ip_pool) // Печать у которых 1.x.x.x
            if (addr.to_bytes()[0]  == 1)
                std::cout << addr << std::endl;

        for(const auto& addr: ip_pool) // Печать у которых 46.70.x.x (0x2E46****)
            if ((addr.to_ulong() & 0xFFFF0000) == 0x2E460000)
                std::cout << addr << std::endl;

        for(const auto& addr: ip_pool) { // Печать у которых любой байт адреса == 46
            auto b_addr = addr.to_bytes();
            if (b_addr[0] == 46 || b_addr[1] == 46 || b_addr[2] == 46 || b_addr[3] == 46)
                std::cout << addr << std::endl;
        }

    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}