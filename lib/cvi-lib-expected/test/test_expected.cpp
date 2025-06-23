#include <gtest/gtest.h>
#include "expected.h"

TEST(ExpectedTest, Construction) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    EXPECT_TRUE(valid.has_value());
    EXPECT_EQ(valid.value(), 42);

    cvi::expected<int, std::string> valid1 = 32;
    EXPECT_TRUE(valid1.has_value());
    EXPECT_EQ(valid1.value(), 32);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    EXPECT_FALSE(invalid.has_value());
    EXPECT_EQ(invalid.error(), "Error");

    cvi::expected<int, std::string> invalid1 = cvi::make_unexpected("Error");
    EXPECT_FALSE(invalid1.has_value());
    EXPECT_EQ(invalid1.error(), "Error");
}

TEST(ExpectedTest, Assignment) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    cvi::expected<int, std::string> another_valid;
    another_valid = valid;
    EXPECT_TRUE(another_valid.has_value());
    EXPECT_EQ(another_valid.value(), 42);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    cvi::expected<int, std::string> another_invalid;
    another_invalid = invalid;
    EXPECT_FALSE(another_invalid.has_value());
    EXPECT_EQ(another_invalid.error(), "Error");
}

TEST(ExpectedTest, Emplace) {
    cvi::expected<std::string, std::string> valid(cvi::in_place, "Hello");
    EXPECT_TRUE(valid.has_value());
    EXPECT_EQ(valid.value(), "Hello");
    
    valid.emplace("World");
    EXPECT_EQ(valid.value(), "World");
}

TEST(ExpectedTest, ValueOr) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    EXPECT_EQ(valid.value_or(0), 42);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    EXPECT_EQ(invalid.value_or(0), 0);
}

TEST(ExpectedTest, AndThen) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    auto result = valid.and_then([](int v) { return cvi::expected<double, std::string>(v * 2.0); });
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 84.0);

    // cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    // float result1 = valid1.and_then([](int v) { return 3.2f; });
    // EXPECT_EQ(result1, 3.2f);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto invalid_result = invalid.and_then([](int v) { return cvi::expected<double, std::string>(v * 2.0); });
    EXPECT_FALSE(invalid_result.has_value());
}

TEST(ExpectedTest, Map) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    auto result = valid.map([](int v) { return v + 1; });
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 43);

    cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    auto result1 = valid1.map([](int v) { return cvi::expected<double, std::string>(v * 2.0); });
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value(), 84.0);

    cvi::expected<int, std::string> valid2 = cvi::expected<int, std::string>(42);
    auto result2 = valid2.map([](int v) { return (v * 2.0); });
    EXPECT_EQ(result2, 84.0);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto invalid_result = invalid.map([](int v) { return v + 1; });
    EXPECT_FALSE(invalid_result.has_value());
}

TEST(ExpectedTest, Transfomer) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    auto result = valid.transform([](int v) { return v + 1; });
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 43);

    cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    auto result1 = valid1.transform([](int v) { return cvi::expected<double, std::string>(v * 2.0); });
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value(), 84.0);

    cvi::expected<int, std::string> valid2 = cvi::expected<int, std::string>(42);
    auto result2 = valid2.transform([](int v) { return (v * 2.0); });
    EXPECT_EQ(result2, 84.0);

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto invalid_result = invalid.transform([](int v) { return v + 1; });
    EXPECT_FALSE(invalid_result.has_value());
}

TEST(ExpectedTest, MapError) {
    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto result = invalid.map_error([](std::string e) { return e + " occurred"; });
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Error occurred");

    cvi::expected<int, std::string> invalid1 = cvi::unexpected<std::string>("Error");
    auto result1 = invalid1.map_error([](std::string e) { return 32; });
    EXPECT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error(), 32);

    cvi::expected<int, std::string> invalid3 = cvi::unexpected<std::string>("Error");
    auto result3 = invalid3.map_error([](std::string e) { return cvi::expected<int, std::string>(32); });
    EXPECT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error(), 32);
}

TEST(ExpectedTest, TransformError) {
    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto result = invalid.transform_error([](std::string e) { return e + " occurred"; });
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Error occurred");

    cvi::expected<int, std::string> invalid1 = cvi::unexpected<std::string>("Error");
    auto result1 = invalid1.transform_error([](std::string e) { return 32; });
    EXPECT_FALSE(result1.has_value());
    EXPECT_EQ(result1.error(), 32);

    cvi::expected<int, std::string> invalid3 = cvi::unexpected<std::string>("Error");
    auto result3 = invalid3.transform_error([](std::string e) { return cvi::expected<int, std::string>(32); });
    EXPECT_FALSE(result3.has_value());
    EXPECT_EQ(result3.error(), 32);
}

TEST(ExpectedTest, Else_Or) {
    cvi::expected<int, std::string> valid = cvi::expected<int, std::string>(42);
    auto result = valid.or_else([](std::string e) { return cvi::expected<int, std::string>(32); });
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 42);

    cvi::expected<int, std::string> invalid1 = cvi::unexpected<std::string>("Error");
    auto result1 = invalid1.or_else([](std::string e) { return cvi::expected<int, std::string>(32); });
    EXPECT_TRUE(result1.has_value());
    EXPECT_EQ(result1.value(), 32);

    cvi::expected<int, std::string> invalid2 = cvi::unexpected<std::string>("Error");
    auto result2 = invalid2.or_else([](std::string e) { });
    EXPECT_FALSE(result2.has_value());
    EXPECT_EQ(result2.error(), "Error");

    cvi::expected<int, std::string> invalid = cvi::unexpected<std::string>("Error");
    auto invalid_result = invalid.or_else([](std::string e) { return cvi::expected<int, std::string>(cvi::unexpect, "testelse"); });
    EXPECT_FALSE(invalid_result.has_value());
    EXPECT_EQ(invalid_result.error(), "testelse");
}

TEST(ExpectedTest, Swap) {
    cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    cvi::expected<int, std::string> valid2 = cvi::expected<int, std::string>(100);
    
    valid1.swap(valid2);

    EXPECT_EQ(valid1.value(), 100);
    EXPECT_EQ(valid2.value(), 42);
}

TEST(ExpectedTest, MoveConstructor) {
    cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    cvi::expected<int, std::string> valid2 = std::move(valid1);

    EXPECT_EQ(valid2.value(), 42);
    // EXPECT_FALSE(valid1.has_value());
    EXPECT_TRUE(valid1.has_value());
    EXPECT_EQ(valid1.value(), 42);
}

TEST(ExpectedTest, MoveAssignment) {
    cvi::expected<int, std::string> valid1 = cvi::expected<int, std::string>(42);
    cvi::expected<int, std::string> valid2;
    
    valid2 = std::move(valid1);

    EXPECT_EQ(valid2.value(), 42);
    // EXPECT_FALSE(valid1.has_value());
    EXPECT_TRUE(valid1.has_value());
    EXPECT_EQ(valid1.value(), 42);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}