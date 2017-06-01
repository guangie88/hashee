/**
 * Provides imbuement functions for rustfp Option type.
 * @author Chen Weiguang
 */

#pragma once

#include "rustfp/option.h"

#include <type_traits>
#include <utility>

namespace hashee {

    // declaration section

    namespace details {
        template <class T>
        class imbue_opt_impl;
    }

    /**
     * Imbues the given type to enable it to become ostream-able.
     * @tparam Opt rustfp::Option specialized type, where the contained type must be ostream-able
     * @param value perfect forwarded optional value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class Opt>
    auto imbue_opt(Opt &&value) -> details::imbue_opt_impl<Opt>;

    // implementation section

    namespace details {
        template <class Opt>
        class imbue_opt_impl {
        public:
            template <class Optx>
            explicit imbue_opt_impl(Optx &&value, decltype(value.is_some()) * = nullptr) :
                value(std::forward<Optx>(value)) {

            }

            /** Provides expression SFINAE to deny non-conforming types */
            template <class... Tx>
            explicit imbue_opt_impl(Tx &&...) {
                static_assert(sizeof...(Tx) == 0, "imbue_opt only works for rustfp::Option<T>.");
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
                    return std::ref(lhs << v);
                },
                [&lhs] {
                    return std::ref(lhs << NONE_STR);
                });
        }
    }

    template <class Opt>
    auto imbue_opt(Opt &&value) -> details::imbue_opt_impl<Opt> {
        return details::imbue_opt_impl<Opt>(std::forward<Opt>(value));
    }
}
