/**
 * Provides all the forwarding declaration of imbuement.
 * @author Chen Weiguang
 */

#pragma once

#ifndef DISABLE_HASHEE_IMBUE_ITER
#include <initializer_list>
#endif

#ifndef DISABLE_HASHEE_IMBUE_RUSTFP
#include "rustfp/option.h"
#endif

#include <type_traits>
#include <utility>

namespace hashee {

    // declaration section for imbue_primitive

    namespace details {
        template <class Primitive>
        class imbue_primitive_impl;
    }

    /**
     * Imbues all primitive types to form the base cases of ostream-able types.
     *
     * Uses express SFINAE to deny other types.
     * @tparam Primitive any std::is_fundamental type
     * @param value primitive type value
     * @return object representing the imbued primitive value
     */
    template <class Primitive>
    auto imbue(
        const Primitive value,
        std::enable_if_t<std::is_fundamental<Primitive>::value> * = nullptr) -> details::imbue_primitive_impl<Primitive>;

    /**
     * Imbues char array since it is not part of the primitive types for ostream-able types.
     * @param chars char array value
     * @return object representing the imbued primitive value
     */
    auto imbue(const char * const chars) -> details::imbue_primitive_impl<const char *>;

#ifndef DISABLE_HASHEE_IMBUE_ITER

    // declaration section for imbue_iter

    namespace details {
        template <class T>
        class imbue_iter_impl;
    }

    /**
     * Imbues the given type to enable it to become ostream-able.
     *
     * Uses express SFINAE to deny other types.
     * @tparam T type of value to be used for imbuement
     * @param value perfect forwarded value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class T>
    auto imbue(
        T &&values,
        decltype(values.begin()) * = nullptr,
        decltype(values.end()) * = nullptr) -> details::imbue_iter_impl<T>;

    /**
     * Imbues the given type to enable it to become ostream-able.
     * @tparam T type of value in std::initializer list used for imbuement, must be ostream-able
     * @param values initializer list values for imbue
     * @return object representing the imbued list of values
     */
    template <class T>
    auto imbue(std::initializer_list<T> values) -> details::imbue_iter_impl<std::initializer_list<T>>;

#endif

#ifndef DISABLE_HASHEE_IMBUE_RUSTFP

    // declaration section for imbue_rustfp

    namespace details {
        template <class Opt>
        class imbue_opt_impl;
    }

    /**
     * Imbues the given rustfp::Option type to enable it to become ostream-able.
     *
     * Uses express SFINAE to deny other types.
     * @tparam Opt rustfp::Option specialized type, where the contained type must be ostream-able
     * @param value perfect forwarded optional value to be used for imbuement
     * @return object representing the imbued value
     */
    template <class Opt>
    auto imbue(Opt &&value, decltype(value.is_some()) * = nullptr) -> details::imbue_opt_impl<Opt>;

#endif
}

// implementation section

#include "imbue_primitive.hpp"
#include "imbue_iter.hpp"
#include "imbue_rustfp.hpp"