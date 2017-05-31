/**
 * Provides imbuement functions for iterable types.
 *
 * Iterable types refer to any class type that contains begin and end methods.
 * @author Chen Weiguang
 */

#pragma once

#include <initializer_list>
#include <utility>

namespace hashee {

    // declaration section

    namespace details {
        template <class T>
        class imbue_iter_impl;
    }

    /**
     * Imbues the given type to enable it to become ostream-able.
     * @tparam T type of value to be used for imbuement
     * @param value perfect forwarded value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class T>
    auto imbue_iter(T &&value) -> details::imbue_iter_impl<T>;

    /**
     * Imbues the given type to enable it to become ostream-able.
     * @tparam T type of value in std::initializer list used for imbuement, must be ostream-able
     * @param values initializer list values for imbue
     * @return object representing the imbued list of values
     */
    template <class T>
    auto imbue_iter(std::initializer_list<T> values) -> details::imbue_iter_impl<std::initializer_list<T>>;

    // implementation section
    
    namespace details {
        template <class T>
        class imbue_iter_impl {
        public:
            /** Special SFINAE based constructor to accept only the iterable type. */
            template <class Tx>
            imbue_iter_impl(
                Tx &&values,
                decltype(values.begin()) * = nullptr,
                decltype(values.end()) * = nullptr) : values(std::forward<Tx>(values)) {

            }

            /** Provides expression SFINAE to deny non-conforming types */
            template <class... Tx>
            explicit imbue_iter_impl(Tx &&...) {
                static_assert(sizeof...(Tx) == 0, "imbue_iter only works for T that has begin and end methods.");
            }

            template <class Tx>
            friend auto operator<<(std::ostream &lhs, const imbue_iter_impl<Tx> &rhs) -> std::ostream &;

        private:
            T values;
        };

        template <class T>
        auto operator<<(std::ostream &lhs, const imbue_iter_impl<T> &rhs) -> std::ostream & {
            for (const auto &v : rhs.values) {
                lhs << v;
            }

            return lhs;
        }
    }

    template <class T>
    auto imbue_iter(T &&values) -> details::imbue_iter_impl<T> {
        return details::imbue_iter_impl<T>(std::forward<T>(values));
    }

    template <class T>
    auto imbue_iter(std::initializer_list<T> values) -> details::imbue_iter_impl<std::initializer_list<T>> {
        return details::imbue_iter_impl<std::initializer_list<T>>(values);
    }
}