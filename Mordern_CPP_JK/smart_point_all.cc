#include <iostream>
#include <utility> // 为了使用 std::exchange 和 std::move
#include <string>

// --- 用于演示的简单类 ---
class MyClass {
public:
    explicit MyClass(const std::string& name) : name_(name) {
        std::cout << "MyClass '" << name_ << "' 构造\n";
    }
    ~MyClass() {
        std::cout << "MyClass '" << name_ << "' 析构\n";
    }
    void sayHello() const {
        std::cout << "Hello from '" << name_ << "'\n";
    }
private:
    std::string name_;
};

// --- 我们的智能指针实现 ---
template <typename T>
class smart_ptr {
public:
    // 1. 默认构造函数
    // 创建一个不管理任何资源的空智能指针
    constexpr smart_ptr() noexcept : ptr_(nullptr) {}

    // 2. 显式构造函数
    // 接收一个裸指针，并接管其所有权。explicit 防止隐式转换。
    explicit smart_ptr(T* p) noexcept : ptr_(p) {}

    // 3. 析构函数
    // 释放所管理的资源。这是 RAII 的核心。
    ~smart_ptr() {
        std::cout << "[析构] 释放资源...\n";
        delete ptr_;
    }

    // 4. 禁用拷贝构造函数和拷贝赋值操作符
    // 这是实现独占所有权的关键。我们不希望两个 smart_ptr 指向同一块内存。
    smart_ptr(const smart_ptr&) = delete;
    smart_ptr& operator=(const smart_ptr&) = delete;

    // 5. 移动构造函数
    // 将资源的所有权从另一个 smart_ptr (rhs) 转移过来。
    // noexcept 告诉编译器这个操作不会抛出异常，这对于标准库容器优化至关重要。
    smart_ptr(smart_ptr&& rhs) noexcept : ptr_(rhs.ptr_) {
        std::cout << "[移动构造] 接管资源所有权\n";
        // 将 rhs 的指针置空，确保 rhs 析构时不会释放已经被我们接管的资源
        rhs.ptr_ = nullptr;
    }

    // 6. 移动赋值操作符
    // 将资源的所有权从另一个 smart_ptr (rhs) 转移过来。
    smart_ptr& operator=(smart_ptr&& rhs) noexcept {
        std::cout << "[移动赋值] 接管资源所有权\n";
        // 1. 防止自赋值 (例如 sp = std::move(sp);)
        if (this != &rhs) {
            // 2. 释放当前已管理的资源
            delete ptr_;
            // 3. 接管 rhs 的资源，并将其置空
            // std::exchange 是一个便捷函数，它将 rhs.ptr_ 的值赋给 ptr_，然后将 rhs.ptr_ 设为 nullptr
            ptr_ = std::exchange(rhs.ptr_, nullptr);
        }
        return *this;
    }

    // 7. 重载解引用操作符 (*)
    // 允许像使用普通指针一样访问对象
    T& operator*() const noexcept {
        return *ptr_;
    }

    // 8. 重载成员访问操作符 (->)
    // 允许像使用普通指针一样访问对象成员
    T* operator->() const noexcept {
        return ptr_;
    }

    // 9. 获取底层裸指针
    // 在某些需要与旧代码交互的场景下有用
    T* get() const noexcept {
        return ptr_;
    }

    // 10. 释放所有权
    // 放弃对资源的管理权，并返回裸指针。之后 smart_ptr 变为空。
    T* release() noexcept {
        return std::exchange(ptr_, nullptr);
    }

    // 11. 重置指针
    // 释放当前资源，并开始管理一个新的资源
    void reset(T* p = nullptr) noexcept {
        delete ptr_;
        ptr_ = p;
    }

    // 12. 检查是否持有资源
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    // 13. swap 成员函数
    // 高效地交换两个 smart_ptr 管理的资源
    void swap(smart_ptr& other) noexcept {
        std::swap(ptr_, other.ptr_);
    }

private:
    T* ptr_; // 管理的裸指针
};

// --- 演示用例 ---

// 一个返回 smart_ptr 的函数，用于演示移动语义
smart_ptr<MyClass> createMyClass(const std::string& name) {
    // 返回一个临时对象，会触发移动构造函数
    return smart_ptr<MyClass>(new MyClass(name));
}

int main() {
    std::cout << "=== 演示 1: 移动构造 ===\n";
    {
        smart_ptr<MyClass> sp1(new MyClass("Object 1"));
        sp1->sayHello();

        // 调用移动构造函数，sp1 的资源所有权转移给 sp2
        // 之后 sp1 变为空
        smart_ptr<MyClass> sp2 = std::move(sp1);
        
        if (!sp1) {
            std::cout << "sp1 现在为空\n";
        }
        sp2->sayHello();
    } // sp2 离开作用域，资源被释放

    std::cout << "\n=== 演示 2: 移动赋值 ===\n";
    {
        smart_ptr<MyClass> sp1(new MyClass("Object 2"));
        smart_ptr<MyClass> sp2(new MyClass("Object 3"));

        // 调用移动赋值操作符
        // sp2 原有的资源 ("Object 3") 被释放
        // sp1 的资源 ("Object 2") 所有权转移给 sp2
        sp2 = std::move(sp1);
        
        if (!sp1) {
            std::cout << "sp1 现在为空\n";
        }
        sp2->sayHello();
    }

    std::cout << "\n=== 演示 3: 函数返回值优化 (RVO/NRVO) ===\n";
    {
        // 编译器通常会优化掉这里的移动构造，直接在 sp 的位置构造对象
        // 但移动构造函数的存在是这种优化能够安全进行的基础
        smart_ptr<MyClass> sp = createMyClass("From Function");
        sp->sayHello();
    }

    return 0;
}