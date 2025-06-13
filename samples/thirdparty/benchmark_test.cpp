#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <cmath>

// ===== Google Test 部分 =====

// 基本测试：加法函数
int Add(int a, int b) {
    return a + b;
}

TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(Add(2, 3), 5);
    EXPECT_EQ(Add(10, 20), 30);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(Add(-2, -3), -5);
    EXPECT_EQ(Add(-10, 20), 10);
}

// 测试夹具：MyClass类
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
    
    // 计算所有数据的平均值
    double CalculateAverage() const {
        if (data.empty()) return 0.0;
        double sum = 0.0;
        for (int val : data) {
            sum += val;
        }
        return sum / data.size();
    }
};

class MyClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        myClass = new MyClass(10);
    }

    void TearDown() override {
        delete myClass;
    }

    MyClass* myClass;
};

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

TEST_F(MyClassTest, AverageTest) {
    myClass->AddData(10);
    myClass->AddData(20);
    myClass->AddData(30);
    EXPECT_DOUBLE_EQ(myClass->CalculateAverage(), 20.0);
}

// 参数化测试：质数判断
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

class IsPrimeParamTest : public ::testing::TestWithParam<int> {
};

INSTANTIATE_TEST_SUITE_P(TrueReturned, IsPrimeParamTest,
                         ::testing::Values(3, 5, 7, 11, 13, 17, 19));

INSTANTIATE_TEST_SUITE_P(FalseReturned, IsPrimeParamTest,
                         ::testing::Values(0, 1, 4, 6, 8, 9, 10, 12));

TEST_P(IsPrimeParamTest, CheckPrime) {
    int n = GetParam();
    if (n >= 2 && n <= 20) {
        EXPECT_EQ(IsPrime(n), n == 3 || n == 5 || n == 7 || n == 11 || 
                  n == 13 || n == 17 || n == 19);
    } else {
        EXPECT_EQ(IsPrime(n), false);
    }
}

// ===== Google Benchmark 部分 =====

// 斐波那契数列：递归实现
int FibonacciRecursive(int n) {
    if (n <= 1) return n;
    return FibonacciRecursive(n-1) + FibonacciRecursive(n-2);
}

// 斐波那契数列：迭代实现
int FibonacciIterative(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// 斐波那契数列基准测试
static void BM_FibonacciRecursive(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        FibonacciRecursive(n);
    }
}

static void BM_FibonacciIterative(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state) {
        FibonacciIterative(n);
    }
}

BENCHMARK(BM_FibonacciRecursive)->Arg(10)->Arg(20)->Arg(30);
BENCHMARK(BM_FibonacciIterative)->Arg(10)->Arg(20)->Arg(30);

// 排序算法基准测试
static void BM_Sort(benchmark::State& state) {
    auto data = std::vector<int>(state.range(0));
    std::generate(data.begin(), data.end(), []() { return rand(); });
    
    for (auto _ : state) {
        auto copy = data;
        std::sort(copy.begin(), copy.end());
        benchmark::DoNotOptimize(copy.data());
        benchmark::ClobberMemory();
    }
    
    state.SetItemsProcessed(state.iterations() * state.range(0));
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(int));
}

BENCHMARK(BM_Sort)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 20)
    ->Complexity();

// 向量元素访问基准测试
static void BM_VectorAccess(benchmark::State& state) {
    std::vector<int> v(state.range(0), 42);
    int sum = 0;
    
    for (auto _ : state) {
        for (int i = 0; i < v.size(); ++i) {
            sum += v[i];
        }
        benchmark::DoNotOptimize(sum);
    }
    
    state.SetItemsProcessed(state.iterations() * v.size());
}

BENCHMARK(BM_VectorAccess)
    ->ArgsProduct({
        benchmark::CreateRange(1<<10, 1<<20, 2),
        benchmark::CreateDenseRange(1, 4, 1)
    });

// 数学计算基准测试
static void BM_Sqrt(benchmark::State& state) {
    double x = 2.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::sqrt(x));
    }
}

static void BM_Pow(benchmark::State& state) {
    double x = 2.0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::pow(x, 2.0));
    }
}

BENCHMARK(BM_Sqrt);
BENCHMARK(BM_Pow);

// 主函数
int main(int argc, char **argv) {
    // 初始化Google Test
    ::testing::InitGoogleTest(&argc, argv);
    
    // 初始化Google Benchmark
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    
    // 运行Google Test
    int test_result = RUN_ALL_TESTS();
    
    // 运行Google Benchmark
    benchmark::RunSpecifiedBenchmarks();
    
    return test_result;
}