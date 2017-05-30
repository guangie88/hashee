#pragma once

#include "rustfp/option.h"

#include <functional>

namespace rustfp {

    // declaration section

    template <class T>
    auto operator<<(std::ostream &lhs, const rustfp::Option<T> &rhs) -> std::ostream &;

    // implementation section

    template <class T>
    auto operator<<(std::ostream &lhs, const rustfp::Option<T> &rhs) -> std::ostream & {
        static constexpr auto NONE_STR = "(NONE)";

        return rhs.match(
            [&lhs](const T &v) {
                return std::ref(lhs << v);
            },
            [&lhs] {
                return std::ref(lhs << NONE_STR);
            });
    }
}