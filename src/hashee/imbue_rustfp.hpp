/**
 * Provides imbuement functions for rustfp Option type.
 * @author Chen Weiguang
 */

#pragma once

#include "rustfp/option.h"

#include <functional>

namespace hashee {

    // declaration section

    namespace details {
        template <class T>
        class imbue_opt_impl;
    }

    /**
     * Imbues the given type to enable it to become ostream-able.
     * @tparam T type of value contained in Option, must be ostream-able
     * @param value optional value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class T>
    auto imbue_opt(const rustfp::Option<T> &value) -> details::imbue_opt_impl<T>;

    // implementation section

    namespace details {
        template <class T>
        class imbue_opt_impl {
        public:
            explicit imbue_opt_impl(const rustfp::Option<T> &value) :
                r(value) {

            }

            template <class Tx>
            friend auto operator<<(std::ostream &lhs, const imbue_opt_impl<Tx> &rhs) -> std::ostream &;

        private:
            std::reference_wrapper<const rustfp::Option<T>> r;
        };

        template <class T>
        auto operator<<(std::ostream &lhs, const imbue_opt_impl<T> &rhs) -> std::ostream & {
            static constexpr auto NONE_STR = " ";

            return rhs.r.get().match(
                [&lhs](const T &v) {
                    return std::ref(lhs << v);
                },
                [&lhs] {
                    return std::ref(lhs << NONE_STR);
                });
        }
    }

    template <class T>
    auto imbue_opt(const rustfp::Option<T> &value) -> details::imbue_opt_impl<T> {
        return details::imbue_opt_impl<T>(value);
    }
}