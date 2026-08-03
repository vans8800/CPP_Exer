#include <iostream>
#include <string>

/*
资源获取：在构造函数中分配堆内存。
自动释放：在对象离开作用域（或发生异常）时，析构函数自动释放内存，无需手动 delete。
防止拷贝（现代 C++ 最佳实践）：防止资源被多次释放。
*/

// 1. 定义一个基类 shape，用于演示多态
class shape {
public:
    virtual ~shape() = default; // 虚析构函数，确保派生类资源被正确释放
    virtual std::string name() const = 0;
};

// 2. 定义一个派生类 circle
class circle : public shape {
public:
    explicit circle(double r) : radius_(r) { //explicit 防止隐式类型转换
        std::cout << "[Heap] circle 对象已创建 (半径: " << radius_ << ")\n";
    }
    ~circle() override {
        std::cout << "[Heap] circle 对象已销毁 (半径: " << radius_ << ")\n";
    }
    std::string name() const override { return "Circle"; }

private:
    double radius_;
};

// 3. 实现 RAII 包装类 shape_wrapper
class shape_wrapper {
public:
    // 构造函数：获取资源（在堆上创建 shape 对象）
    explicit shape_wrapper(shape* p = nullptr) : ptr_(p) {
        std::cout << "[RAII] shape_wrapper 构造，接管资源。\n";
    }

    // 析构函数：自动释放资源（这是 RAII 的核心）
    ~shape_wrapper() {
        std::cout << "[RAII] shape_wrapper 析构，准备释放资源...\n";
        delete ptr_; // 即使 ptr_ 为 nullptr，delete 也是安全的
    }

    // 禁用拷贝构造和拷贝赋值，防止多个 wrapper 释放同一块内存
    shape_wrapper(const shape_wrapper&) = delete;
    shape_wrapper& operator=(const shape_wrapper&) = delete;

    // 提供获取底层指针的方法，以便调用 shape 的方法
    shape* get() const { return ptr_; }

private:
    shape* ptr_;
};

// 4. 演示正常作用域下的 RAII
void demo_normal_scope() {
    std::cout << "=== 演示 1: 正常作用域 ===\n";
    // 在栈上创建 wrapper，并在构造时传入堆上的 circle
    shape_wrapper wrapper(new circle(5.0)); 
    
    // 使用资源
    std::cout << "当前形状: " << wrapper.get()->name() << "\n";
    
    // 函数结束，wrapper 离开作用域，自动调用析构函数释放 circle
    std::cout << "即将离开 demo_normal_scope 函数...\n";
}

// 5. 演示异常安全下的 RAII
void demo_exception_safety() {
    std::cout << "\n=== 演示 2: 异常安全 ===\n";
    try {
        shape_wrapper wrapper(new circle(3.14)); // 在堆上创建 circle，并由 wrapper 管理; 将资源生命周期绑定到 wrapper 栈对象上
        std::cout << "当前形状: " << wrapper.get()->name() << "\n";
        
        // 模拟业务逻辑中抛出异常
        throw std::runtime_error("发生了一个致命错误！");
        
        // 注意：这里永远不会执行到 delete wrapper.get()
    } catch (const std::exception& e) {
        std::cout << "捕获到异常: " << e.what() << "\n";
        std::cout << "尽管发生了异常，堆内存依然被 RAII 正确释放了！\n";
    }
}

int main() {
    // demo_normal_scope();
    
    demo_exception_safety();
    
    std::cout << "\n=== 程序结束 ===\n";
    return 0;
}