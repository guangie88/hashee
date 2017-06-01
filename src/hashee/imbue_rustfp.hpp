/**
 * Provides imbuement functions for rustfp Option type.
 * @author Chen Weiguang
 */

#pragma once

#ifndef DISABLE_HASHEE_IMBUE_RUSTFP

#include "rustfp/option.h"

#include <type_traits>
#include <utility>

namespace hashee {

    // implementation section

    namespace details {
        template <class Opt>
        class imbue_opt_impl {
        public:
            template <class Optx>
            explicit imbue_opt_impl(Optx &&value, decltype(value.is_some()) * = nullptr) :
                value(std::forward<Optx>(value)) {

            }

            template <class Optx>
            friend auto operator<<(std::ostream &lhs, const imbue_opt_impl<Optx> &rhs) -> std::ostream &;

        private:
            Opt value;
        };

        template <class Opt>
        auto operator<<(std::ostream &lhs, const imbue_opt_impl<Opt> &rhs) -> std::ostream & {
            using some_t = typename std::remove_const_t<std::remove_reference_t<Opt>>::some_t;
            static constexpr auto NONE_STR = " ";

            return rhs.value.match(
                [&lhs](const some_t &v) {
                    return std::ref(lhs << imbue(v));
                },
                [&lhs] {
                    return std::ref(lhs << NONE_STR);
                });
        }
    }

    template <class Opt>
    auto imbue(Opt &&value, decltype(value.is_some()) *) -> details::imbue_opt_impl<Opt> {
        return details::imbue_opt_impl<Opt>(std::forward<Opt>(value));
    }
}

#endif