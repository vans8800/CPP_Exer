#include <iostream>
#include <cstring>
#include <utility>

class MyString {
private:
    char* m_buffer{nullptr};
    size_t m_length{0};

public:
    // ----------------------------------------------------
    // 1. 基础构造函数 (Default & C-string Constructor)
    // ----------------------------------------------------
    MyString(const char* str = "") {
        if (str) {
            m_length = std::strlen(str);
            m_buffer = new char[m_length + 1];
            std::strcpy(m_buffer, str);
        } else {
            m_buffer = new char[1]{'\0'};
            m_length = 0;
        }
        std::cout << "[构造函数] 创建: \"" << m_buffer << "\"\n";
    }

    // ----------------------------------------------------
    // 2. 拷贝构造函数 (Copy Constructor - 左值)
    // ----------------------------------------------------
    MyString(const MyString& other) : m_length(other.m_length) {
        m_buffer = new char[m_length + 1];
        std::strcpy(m_buffer, other.m_buffer);
        std::cout << "[拷贝构造] 深拷贝: \"" << m_buffer << "\"\n";
    }

    // ----------------------------------------------------
    // 3. 移动构造函数 (Move Constructor - 右值)
    // ----------------------------------------------------
    MyString(MyString&& other) noexcept 
        : m_buffer(other.m_buffer), m_length(other.m_length) {
        // 接管/偷走资源
        other.m_buffer = nullptr;
        other.m_length = 0;
        std::cout << "[移动构造] 偷取资源成功!\n";
    }

    // ----------------------------------------------------
    // 4. 拷贝赋值运算符 (Copy Assignment - 左值)
    // ----------------------------------------------------
    MyString& operator=(const MyString& other) {
        std::cout << "[拷贝赋值] 执行深拷贝赋值\n";
        if (this != &other) {
            delete[] m_buffer; // 释放旧内存

            m_length = other.m_length;
            m_buffer = new char[m_length + 1];
            std::strcpy(m_buffer, other.m_buffer);
        }
        return *this;
    }

    // ----------------------------------------------------
    // 5. 移动赋值运算符 (Move Assignment - 右值)
    // ----------------------------------------------------
    MyString& operator=(MyString&& other) noexcept {
        std::cout << "[移动赋值] 执行资源移动赋值\n";
        if (this != &other) {
            delete[] m_buffer; // 释放旧内存

            // 偷取资源
            m_buffer = other.m_buffer;
            m_length = other.m_length;
            // 缺少std::strcpy，因为我们直接接管了指针，不需要复制内容

            // 将 other 置为安全状态
            other.m_buffer = nullptr;
            other.m_length = 0;
        }
        return *this;
    }

    // ----------------------------------------------------
    // 6. 析构函数 (Destructor)
    // ----------------------------------------------------
    ~MyString() {
        delete[] m_buffer;
    }

    // 辅助函数：内部追加字符串（用于 + 运算符优化）
    void append(const MyString& other) {
        if (other.m_length == 0) return;
        
        size_t newLength = m_length + other.m_length;
        char* newBuffer = new char[newLength + 1];

        if (m_buffer) std::strcpy(newBuffer, m_buffer);
        std::strcat(newBuffer, other.m_buffer);

        delete[] m_buffer;
        m_buffer = newBuffer;
        m_length = newLength;
    }

    // 打印辅助
    void print() const {
        if (m_buffer) {
            std::cout << m_buffer;
        } else {
            std::cout << "(null/moved-from)";
        }
    }

    // ----------------------------------------------------
    // 7. operator+ 重载优化（关键部分）
    // ----------------------------------------------------

    // 情况 A：左值 + 左值 (必须分配新内存)
    friend MyString operator+(const MyString& lhs, const MyString& rhs) {
        std::cout << "  -> [operator+ (左值 + 左值)]\n";
        MyString result(lhs); // 先深拷贝左值
        result.append(rhs);   // 追加右值
        return result;        // 触发 RVO 或移动返回
    }

    // 情况 B：右值 + 左值 (如: (Hello + World) + CPP )
    // 这里的 lhs 是一个临时对象（右值），可以直接复用它的内存！无需重复重新分配！
    friend MyString operator+(MyString&& lhs, const MyString& rhs) {
        std::cout << "  -> [operator+ (右值 + 左值 - 性能优化!)]\n";
        lhs.append(rhs);       // 直接在临时对象的 buffer 后追加，零额外拷贝
        return std::move(lhs); // 移动返回
    }

    // 友元函数用于 std::cout <<
    friend std::ostream& operator<<(std::ostream& os, const MyString& str) {
        if (str.m_buffer) os << str.m_buffer;
        return os;
    }
};

// ========================================================
// 测试用例：复现你的场景并观察打印日志
// ========================================================
int main() {
    std::cout << "=== 1. 初始化基本变量 ===\n";
    MyString Hello("Hello ");
    MyString World("World");
    MyString CPP(" of C++");

    std::cout << "\n=== 2. 测试链式相加构造: MyString sayHello(Hello + World + CPP); ===\n";
    // 计算过程分析：
    // Step 1: Hello + World  -> (左值+左值)，生成临时对象 Temp1 ("Hello World")
    // Step 2: Temp1 + CPP    -> (右值+左值)，触发右值优化 operator+，复用 Temp1 内存追加 " of C++"
    // Step 3: 构造 sayHello   -> 直接接管 Temp1 内存（或者通过 C++17 强制省略拷贝）
    MyString sayHello(Hello + World + CPP);
    std::cout << "结果 sayHello = " << sayHello << "\n";
  
    std::cout << std::endl;

    std::cout << "\n=== 3. 测试链式相加赋值: sayHelloAgain = Hello + World + CPP; ===\n";
    MyString sayHelloAgain("overwrite this");
    sayHelloAgain = Hello + World + CPP; // 触发移动赋值 (Move Assignment)
    std::cout << "结果 sayHelloAgain = " << sayHelloAgain << "\n";

    std::cout << "\n=== 4. 程序结束，析构变量 ===\n";
    return 0;
}