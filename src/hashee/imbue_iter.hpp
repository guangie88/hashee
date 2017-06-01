/**
 * Provides imbuement functions for iterable types.
 *
 * Iterable types refer to any class type that contains begin and end methods.
 * @author Chen Weiguang
 */

#pragma once

#ifndef DISABLE_HASHEE_IMBUE_ITER

#include <initializer_list>
#include <utility>

namespace hashee {

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

            template <class Tx>
            friend auto operator<<(std::ostream &lhs, const imbue_iter_impl<Tx> &rhs) -> std::ostream &;

        private:
            T values;
        };

        template <class T>
        auto operator<<(std::ostream &lhs, const imbue_iter_impl<T> &rhs) -> std::ostream & {
            for (const auto &v : rhs.values) {
                lhs << imbue(v);
            }

            return lhs;
        }
    }

    template <class T>
    auto imbue(
        T &&values,
        decltype(values.begin()) *,
        decltype(values.end()) *) -> details::imbue_iter_impl<T> {

        return details::imbue_iter_impl<T>(std::forward<T>(values));
    }

    template <class T>
    auto imbue(std::initializer_list<T> values) -> details::imbue_iter_impl<std::initializer_list<T>> {
        return details::imbue_iter_impl<std::initializer_list<T>>(values);
    }
}

#endif