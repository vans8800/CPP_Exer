#include <iostream>
#include <cstring>

// 1. 通用类模板
template <typename T>
class Comparer {
public:
    static bool isEqual(T a, T b) {
        std::cout << "[通用版本] ";
        return a == b;
    }
};

// 2. 模板的全特化 (Full Specialization)
// 专门针对 const char* 类型进行定制
template <>
class Comparer<const char*> {
public:
    static bool isEqual(const char* a, const char* b) {
        std::cout << "[特化版本 - C风格字符串] ";
        return std::strcmp(a, b) == 0;
    }
};

int main() {
    // 使用通用模板实例化
    std::cout << Comparer<int>::isEqual(10, 10) << std::endl; 
    // 输出: [通用版本] 1

    // 使用具体化（特化）版本
    const char* str1 = "hello";
    const char* str2 = "hello";
    std::cout << Comparer<const char*>::isEqual(str1, str2) << std::endl; 
    // 输出: [特化版本 - C风格字符串] 1

    return 0;
}