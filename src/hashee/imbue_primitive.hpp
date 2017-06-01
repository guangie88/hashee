/**
 * Provides imbuement functions for primitives to start off the base cases.
 * @author Chen Weiguang
 */

#pragma once

#include <type_traits>

namespace hashee {

    // implementation section

    namespace details {
        template <class Primitive>
        class imbue_primitive_impl {
        public:
            imbue_primitive_impl(const Primitive value) :
                value(value) {

            }

            template <class Primitivex>
            friend auto operator<<(std::ostream &lhs, const imbue_primitive_impl<Primitivex> &rhs) -> std::ostream &;
        
        private:
            Primitive value;
        };

        template <class Primitivex>
        auto operator<<(std::ostream &lhs, const imbue_primitive_impl<Primitivex> &rhs) -> std::ostream & {
            return lhs << rhs.value;
        }
    }

    template <class Primitive>
    auto imbue(
        const Primitive value,
        std::enable_if_t<std::is_fundamental<Primitive>::value> *) -> details::imbue_primitive_impl<Primitive> {

        return details::imbue_primitive_impl<Primitive>(value);
    }

    inline auto imbue(const char * const chars) -> details::imbue_primitive_impl<const char *> {
        return details::imbue_primitive_impl<const char *>(chars);
    }
}