/**
 * Contains common utility functions across the namespace.
 * @author Chen Weiguang
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace hashee {
    
    // declaration section

    template <size_t N>
    auto bytes_to_hex(const std::array<uint8_t, N> &bytes) -> std::string;

    // implementation section

    namespace details {
        inline auto char_to_hex(const char c) -> char {
            switch (c) {
            case 0: return '0';
            case 1: return '1';
            case 2: return '2';
            case 3: return '3';
            case 4: return '4';
            case 5: return '5';
            case 6: return '6';
            case 7: return '7';
            case 8: return '8';
            case 9: return '9';
            case 10: return 'a';
            case 11: return 'b';
            case 12: return 'c';
            case 13: return 'd';
            case 14: return 'e';
            case 15: return 'f';
            }
        }
    }

    template <size_t N>
    auto bytes_to_hex(const std::array<uint8_t, N> &bytes) -> std::string {
        static constexpr auto HEX_BASE = 16;
        std::string str(N * 2, '\0');

        for (size_t i = 0; i < N; ++i) {
            str[i * 2] = details::char_to_hex(bytes[i] / HEX_BASE);
            str[i * 2 + 1] = details::char_to_hex(bytes[i] % HEX_BASE);
        }

        return str;
    }
}