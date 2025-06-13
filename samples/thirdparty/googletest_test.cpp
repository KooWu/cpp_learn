#include <gtest/gtest.h>
#include <vector>

// ===== 基本测试部分 =====

// 被测试的函数
int Add(int a, int b) {
    return a + b;
}

// 基本测试用例
TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(Add(2, 3), 5);
    EXPECT_EQ(Add(10, 20), 30);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(Add(-2, -3), -5);
    EXPECT_EQ(Add(-10, 20), 10);
}

// ===== 测试夹具部分 =====

// 被测试的类
class MyClass {
private:
    int value;
    std::vector<int> data;

public:
    MyClass(int val) : value(val) {}
    
    void SetValue(int val) { value = val; }
    int GetValue() const { return value; }
    int Add(int val) { return value += val; }
    
    void AddData(int val) { data.push_back(val); }
    size_t DataSize() const { return data.size(); }
    bool IsDataEmpty() const { return data.empty(); }
};

// 测试夹具类
class MyClassTest : public ::testing::Test {
protected:
    // 每个测试用例开始前执行
    void SetUp() override {
        myClass = new MyClass(10);
    }

    // 每个测试用例结束后执行
    void TearDown() override {
        delete myClass;
    }

    // 测试中使用的对象
    MyClass* myClass;
};

// 使用测试夹具的测试用例
TEST_F(MyClassTest, GetValueTest) {
    EXPECT_EQ(myClass->GetValue(), 10);
}

TEST_F(MyClassTest, SetValueTest) {
    myClass->SetValue(20);
    EXPECT_EQ(myClass->GetValue(), 20);
}

TEST_F(MyClassTest, AddTest) {
    EXPECT_EQ(myClass->Add(5), 15);
    EXPECT_EQ(myClass->GetValue(), 15);
}

TEST_F(MyClassTest, DataTest) {
    EXPECT_TRUE(myClass->IsDataEmpty());
    myClass->AddData(1);
    myClass->AddData(2);
    EXPECT_EQ(myClass->DataSize(), 2);
}

// ===== 参数化测试部分 =====

// 被测试的函数
bool IsPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

// 参数化测试
class IsPrimeParamTest : public ::testing::TestWithParam<int> {
};

// 使用INSTANTIATE_TEST_SUITE_P宏定义测试参数
INSTANTIATE_TEST_SUITE_P(TrueReturned, IsPrimeParamTest,
                         ::testing::Values(3, 5, 7, 11, 13, 17, 19));

INSTANTIATE_TEST_SUITE_P(FalseReturned, IsPrimeParamTest,
                         ::testing::Values(0, 1, 4, 6, 8, 9, 10, 12));

// 测试用例
TEST_P(IsPrimeParamTest, CheckPrime) {
    int n = GetParam();
    if (n >= 2 && n <= 20) {
        EXPECT_EQ(IsPrime(n), n == 3 || n == 5 || n == 7 || n == 11 || 
                  n == 13 || n == 17 || n == 19);
    } else {
        EXPECT_EQ(IsPrime(n), false);
    }
}

// ===== 主函数 =====
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}