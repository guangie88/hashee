#include "gtest/gtest.h"

#include "rustfp/option.h"

#include "hashee/sha1.hpp"

#include <array>
#include <string>
#include <utility>
#include <vector>

// rustfp
using rustfp::Option;
using rustfp::None;
using rustfp::Some;

// hashee
using hashee::sha1_digest;
using hashee::sha1_digest_hex;
using hashee::sha1_msg;

// std
using std::array;
using std::move;
using std::string;
using std::vector;

TEST(Hashee, Sha1Empty) {
    // empty string SHA-1 from online
    EXPECT_EQ("da39a3ee5e6b4b0d3255bfef95601890afd80709", sha1_digest_hex(""));
}

TEST(Hashee, Sha1HelloWorldChain) {
    // SHA-1 from online
    EXPECT_EQ("2ef7bde608ce5404e97d5f042f95f89f1c232871", sha1_digest_hex("Hello World!"));
}

TEST(Hashee, Sha1HelloWorld) {
    // SHA-1 from online
    sha1_msg msg{};
    msg << "Hello" << " " << "World" << "!";
    EXPECT_EQ("2ef7bde608ce5404e97d5f042f95f89f1c232871", msg.digest_hex());
}

TEST(Hashee, Sha1Vec) {
    // imbued vector that gets printed like as if all the values are appended to each other
    EXPECT_EQ(sha1_digest_hex("Hello World!"),
        sha1_digest_hex(vector<string>{"Hello ", "World", "!"}));
}

TEST(Hashee, Sha1Arr) {
    // the pretty printing appends comma separation
    EXPECT_EQ(sha1_digest_hex("How are you today?"),
        sha1_digest_hex(array<const char *, 4>{"How ", "are ", "you ", "today?"}));
}

TEST(Hashee, Sha1InitList) {
    // works for initializer list as well
    EXPECT_EQ(sha1_digest_hex("abcdefghijklmno"),
        sha1_digest_hex({"abc", "def", "ghi", "jkl", "mno"}));
}
 
TEST(Hashee, Sha1VecArrChain) {
    EXPECT_EQ(sha1_digest_hex("123, 3.14, 1.57"),
        sha1_digest_hex(
            vector<int>{1, 2, 3},
            ", ",
            array<double, 1>{3.14},
            ", ",
            array<float, 1>{1.57f}));
}

TEST(Hashee, Sha1RustfpSome) {
    // SHA-1 from online
    EXPECT_EQ("ebfdc55b4b7eddbb7306eee878315b0df4fde64e",
        sha1_digest_hex(
            Some(777),
            Some(888),
            Some(999)));
}

TEST(Hashee, Sha1RustfpNone) {
    // as long as None does not result in empty string hash, it is acceptable
    EXPECT_NE("da39a3ee5e6b4b0d3255bfef95601890afd80709",
        sha1_digest_hex(Option<int>(None)));
}

TEST(Hashee, Sha1WriteString) {
    // binary SHA-1 from online
    // one byte is fine for different endian
    sha1_msg msg{};
    const auto val = 'A';
    msg.write(&val, sizeof(val));

    EXPECT_EQ("6dcd4ce23d88e2ee9568ba546c007c63d9131c1b", msg.digest_hex());
}

TEST(Hashee, Sha1Nested) {
    const auto hash = sha1_digest_hex({
            vector<int>{123, 456},
            vector<int>{789}
        },
        "abc",
        Some("def"),
        Option<vector<string>>(Some(vector<string>{"ghi", "jkl"})),
        vector<Option<string>>{Some(string("mno"))},
        "");

    EXPECT_EQ(sha1_digest_hex("123456789abcdefghijklmno"), hash);
}

int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
