#include <iostream>
#include <string>
#include <cstdint>
#include <cstdio>

int main() {
    std::string name;

    std::cout << "name: ";
    std::getline(std::cin, name);

    uint32_t hash = 0x811C9DC5;

    for (size_t i = 0; i < name.length(); i++) {
        hash = (hash ^ static_cast<uint8_t>(name[i])) * 0x01000193;
    }


    int16_t length = static_cast<int16_t>(name.length());


    uint32_t original_hash = hash;


    uint32_t uVar12 =
        static_cast<uint16_t>(length * 0x1234) ^ original_hash;


    hash = uVar12 * 3 + (original_hash >> 11);

    uint16_t part1 = uVar12 & 0xFFFF;
    uint16_t part2 = hash & 0xFFFF;
    uint16_t part3 =
        (uVar12 & 0xFFFF) ^
        (hash & 0xFFFF) ^
        0xC0DE;


    printf("Generated serial: %04X-%04X-%04X\n",
           part1, part2, part3);

    return 0;
}
