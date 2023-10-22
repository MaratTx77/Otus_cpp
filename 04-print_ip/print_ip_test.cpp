#include <gtest/gtest.h>
#include "print_ip.h"


TEST(TestIntTypes, Subtest_Byte) {
    ASSERT_TRUE(to_string(int8_t{-1}) == std::string{"255"}) ;
}

TEST(TestIntTypes, Subtest_Word) {
    ASSERT_TRUE(to_string(int16_t{0}) == std::string{"0.0"}) ;
}

TEST(TestIntTypes, Subtest_4Byte) {
    ASSERT_TRUE(to_string(int32_t{2130706433}) == std::string{"127.0.0.1"}) ;
}

TEST(TestIntTypes, Subtest_8Byte) {
    ASSERT_TRUE(to_string(int64_t{8875824491850138409}) == std::string{"123.45.67.89.101.112.131.41"}) ;
}

TEST(TestStringTypes, Subtest_Std_String) {
    ASSERT_TRUE(to_string(std::string{"Hello, World!"}) == std::string{"Hello, World!"}) ;
}

TEST(TestStringTypes, Subtest_Std_StringView) {
    ASSERT_TRUE(to_string(std::string_view {"Hello, World!"}) == std::string{"Hello, World!"}) ;
}

TEST(TestSTLTypes, Subtest_Std_Vector) {
    ASSERT_TRUE(to_string(std::vector<int>{100, 200, 300, 400} ) == std::string{"100.200.300.400"}) ;
}

TEST(TestSTLTypes, Subtest_Std_List) {
    ASSERT_TRUE(to_string(std::list<short>{400, 300, 200, 100} ) == std::string{"400.300.200.100"}) ;
}

TEST(TestSTLTypes, Subtest_Std_Tupple_Empty) {
    ASSERT_TRUE(to_string(std::make_tuple()) == std::string{""}) ;
}

TEST(TestSTLTypes, Subtest_Std_Tupple) {
    ASSERT_TRUE(to_string(std::make_tuple(123, 456, 789, 0)) == std::string{"123.456.789.0"}) ;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}