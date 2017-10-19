/**
 * Contains SHA-1 convenient c++ functions and structure around the SHA-1 algorithm.
 * @author Chen Weiguang
 */

#pragma once

#include "imbue.hpp"
#include "util.hpp"

#include "openssl/sha.h"

#include <array>
#include <cstdint>
#include <initializer_list>
#include <ostream>
#include <sstream>
#include <string>

namespace hashee {

    // declaration section

    /**
     * Holds the message to be SHA-1 digested
     */
    class sha1_msg {
    public:
        /**
         * Appends new ostream-able value into the digest.
         * @tparam T ostream-able type
         * @param val ostream-able value
         * @return *this
         */
        template <class T>
        auto operator<<(T &&val) -> sha1_msg &;

        /**
         * Appends ostream writable value into the digest.
         * @tparam T ostream writable type
         * @param ptr ostream writable value address
         * @param len number of bytes to write from the value address position
         * @return *this
         */
        template <class T>
        auto write(const T * const ptr, const size_t len) -> sha1_msg &;

        /**
         * Digests all the accumulated message.
         * @return SHA-1 message digest
         */
        auto digest() const -> std::array<uint8_t, SHA_DIGEST_LENGTH>;

        /**
         * Digests all the accumulated message.
         * @return SHA-1 message digest in hex format
         */
        auto digest_hex() const -> std::string;

    private:
        std::stringstream buf;
    };

    /**
     * Directly digest all the given ostream-able values.
     * @tparam T ostream-able type
     * @tparam Ts other ostream-able types
     * @param val ostream-able value
     * @param vals other ostream-able values
     * @return SHA-1 message digest
     */
    template <class T, class... Ts>
    auto sha1_digest(T &&val, Ts &&... vals) -> std::array<uint8_t, SHA_DIGEST_LENGTH>;

    /**
     * Directly digest all the given ostream-able values using std::initializer_list.
     * @tparam T ostream-able type
     * @tparam Ts other ostream-able types
     * @param lst std::initializer_list of ostream-able values
     * @param vals other ostream-able values
     * @return SHA-1 message digest
     */
    template <class T, class... Ts>
    auto sha1_digest(std::initializer_list<T> lst, Ts &&... vals) -> std::array<uint8_t, SHA_DIGEST_LENGTH>;

    /**
     * Directly digest all the given ostream-able values.
     * @tparam T ostream-able type
     * @tparam Ts other ostream-able types
     * @param val ostream-able value
     * @param vals other ostream-able values
     * @return SHA-1 message digest in hex format
     */
    template <class T, class... Ts>
    auto sha1_digest_hex(T &&val, Ts &&... vals) -> std::string;

    /**
     * Directly digest all the given ostream-able values using std::initializer_list.
     * @tparam T ostream-able type
     * @tparam Ts other ostream-able types
     * @param lst std::initializer_list of ostream-able values
     * @param vals other ostream-able values
     * @return SHA-1 message digest in hex format
     */
    template <class T, class... Ts>
    auto sha1_digest_hex(std::initializer_list<T> lst, Ts &&... vals) -> std::string;
    
    // implementation section

    namespace details {
        inline auto sha1_digest_impl(sha1_msg &msg) -> std::array<uint8_t, SHA_DIGEST_LENGTH> {
            return msg.digest();
        }

        template <class T, class... Ts>
        auto sha1_digest_impl(sha1_msg &msg, T &&val, Ts &&... vals) -> std::array<uint8_t, SHA_DIGEST_LENGTH> {
            msg << std::forward<T>(val);
            return sha1_digest_impl(msg, std::forward<Ts>(vals)...);
        }
    }

    template <class T>
    auto sha1_msg::operator<<(T &&val) -> sha1_msg & {
        buf << imbue(std::forward<T>(val));
        return *this;
    }

    inline auto sha1_msg::digest() const -> std::array<uint8_t, SHA_DIGEST_LENGTH> {
        const auto str = buf.str();
        std::array<uint8_t, SHA_DIGEST_LENGTH> hash;

        SHA1(reinterpret_cast<const unsigned char *>(str.c_str()),
            str.length(),
            hash.data());

        return hash;
    }

    template <class T>
    auto sha1_msg::write(const T * const ptr, const size_t len) -> sha1_msg & {
        buf.write(reinterpret_cast<const char *>(ptr), len);
        return *this;
    }

    inline auto sha1_msg::digest_hex() const -> std::string {
        return bytes_to_hex(digest());
    }

    template <class T, class... Ts>
    auto sha1_digest(T &&val, Ts &&... vals) -> std::array<uint8_t, SHA_DIGEST_LENGTH> {
        sha1_msg msg{};
        return details::sha1_digest_impl(msg, std::forward<T>(val), std::forward<Ts>(vals)...);
    }

    template <class T, class... Ts>
    auto sha1_digest(std::initializer_list<T> lst, Ts &&... vals) -> std::array<uint8_t, SHA_DIGEST_LENGTH> {
        // initializer list must not be passed by reference
        sha1_msg msg{};
        return details::sha1_digest_impl(msg, lst, std::forward<Ts>(vals)...);
    }

    template <class T, class... Ts>
    auto sha1_digest_hex(T &&val, Ts &&... vals) -> std::string {
        return bytes_to_hex(sha1_digest(std::forward<T>(val), std::forward<Ts>(vals)...));
    }

    template <class T, class... Ts>
    auto sha1_digest_hex(std::initializer_list<T> lst, Ts &&... vals) -> std::string {
        // initializer list must not be passed by reference
        return bytes_to_hex(sha1_digest(lst, std::forward<Ts>(vals)...));
    }
}
