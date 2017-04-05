#include <gtest/gtest.h>
#include "MD5.hpp"

TEST(Md5EncryptTest, EmptyInput) {
    EXPECT_STREQ("d41d8cd98f00b204e9800998ecf8427e", MD5::encrypt("").c_str());
}

TEST(Md5EncryptTest, SimpleString) {
    EXPECT_STREQ("9e107d9d372bb6826bd81d3542a419d6", MD5::encrypt("The quick brown fox jumps over the lazy dog").c_str());
    EXPECT_STREQ("1055d3e698d289f2af8663725127bd4b", MD5::encrypt("The quick brown fox jumps over the lazy cog").c_str());
}

TEST(Md5EncryptTest, LongString) {
    std::string longString= "";
    for (int i = 1; i <= 10; i++)
        longString += "abcdefghijklmnopqrstuvwxyz";
    EXPECT_STREQ("4e6405697346169610a3a39991c48321", MD5::encrypt(longString).c_str());
}
