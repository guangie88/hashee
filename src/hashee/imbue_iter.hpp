/**
 * Provides imbuement functions for iterable types.
 *
 * Iterable types refer to any class type that contains begin and end methods.
 * @author Chen Weiguang
 */

#pragma once

#include <functional>
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
     * @param value value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class T>
    auto imbue_iter(const T &value) -> details::imbue_iter_impl<T>;

    // implementation section
    
    namespace details {
        template <class T>
        class imbue_iter_impl {
        public:
            /** Special SFINAE based constructor to accept only the iterable type. */
            explicit imbue_iter_impl(
                const T &value,
                decltype(value.begin()) * = nullptr,
                decltype(value.end()) * = nullptr) : r(value) {

            }

            /** Provides expression SFINAE to deny non-conforming types */
            template <class... Tx>
            explicit imbue_iter_impl(Tx &&...) {
                static_assert(sizeof...(Tx) == 0, "Imbue only works for T that has begin and end methods.");
            }

            template <class Tx>
            friend auto operator<<(std::ostream &lhs, const imbue_iter_impl<Tx> &rhs) -> std::ostream &;

        private:
            std::reference_wrapper<const T> r;
        };

        template <class T>
        auto operator<<(std::ostream &lhs, const imbue_iter_impl<T> &rhs) -> std::ostream & {
            for (const auto &v : rhs.r.get()) {
                lhs << v;
            }

            return lhs;
        }
    }

    template <class T>
    auto imbue_iter(const T &value) -> details::imbue_iter_impl<T> {
        return details::imbue_iter_impl<T>(value);
    }
}