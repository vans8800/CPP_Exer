#include <iostream>
#include <string>
#include <utility> // std::move

// 【辅助工具】类型名称打印器
// 用于在编译期查看模板推导出的真实类型
template <typename T>
struct TypePrinter; // 故意不定义实现，只在编译错误时暴露类型

// 我们的测试类
class Widget {
public:
    explicit Widget(const std::string& name) : name_(name) {
        std::cout << "Widget [" << name_ << "] 构造\n";
    }
    ~Widget() {
        std::cout << "Widget [" << name_ << "] 析构\n";
    }
    std::string name_;
};

// 【核心演示】万能引用与引用坍缩
template <typename T>
void process(T&& param) {
    std::cout << "========================================\n";
    
    // 1. 打印 T 的推导类型（受引用坍缩影响）
    std::cout << "T 的推导类型: ";
    TypePrinter<T>(); // 故意触发编译错误来查看类型
    
    // 2. 打印 param 的实际物理类型
    std::cout << "param 的实际物理类型: ";
    TypePrinter<decltype(param)>(); // 故意触发编译错误来查看类型

}

int main() {
    Widget w("MyWidget"); // 1. 创建一个左值对象

    // 场景 A: 传入左值
    process(w); 
    
    // 场景 B: 传入右值
    process(Widget("TempWidget")); 
    
    // 场景 C: 传入 std::move 的结果
    process(std::move(w)); 

    // 场景 D: 右值引用变量本身是左值！
    Widget&& rref = Widget("RRefWidget"); // rref 是右值引用类型
    process(rref); // 但作为表达式传入时，它是左值！

    return 0;
}