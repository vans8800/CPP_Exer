### **1. 对象切片（Object Slicing）**

**陷阱描述**：当将派生类对象赋值或按值传递给基类对象时，派生类特有的成员会被“切掉”，只保留基类部分。这会导致多态彻底失效。
**规避方案**：永远使用基类的**指针**或**引用**来操作派生类对象。

```cpp
class Base {
public:
    virtual void Show() const { std::cout << "Base\n"; }
};
class Derived : public Base {
public:
    void Show() const override { std::cout << "Derived\n"; }
};

void Test(Base b) { b.Show(); } // 按值传递：触发对象切片

int main() {
    Derived d;
    Base b = d; // 对象切片：b 只是 Base 类型
    b.Show();   // 输出: Base
    Test(d);    // 输出: Base
    
    // 正确做法：使用指针或引用
    Base& ref = d;
    ref.Show(); // 输出: Derived
}
```

### **2. 非虚析构函数导致内存泄漏**

**陷阱描述**：如果基类的析构函数不是虚函数，当通过基类指针删除派生类对象时，只会调用基类的析构函数，派生类的析构函数不会被调用，导致派生类中分配的资源无法释放318。
**规避方案**：只要一个类包含虚函数（即作为多态基类），其析构函数**必须**声明为 `virtual`。

```cpp
class Base {
public:
    ~Base() { std::cout << "Base destroyed\n"; } // 危险！非虚析构
};
class Derived : public Base {
    int* data;
public:
    Derived() : data(new int) {}
    ~Derived() { 
        delete[] data; 
        std::cout << "Derived destroyed\n"; 
    }
};

int main() {
    Base* p = new Derived();
    delete p; // 仅输出: Base destroyed，data 内存泄漏！
}
```

### **3. 漏写** `override` **导致的静默隐藏**

**陷阱描述**：派生类重写基类虚函数时，如果函数名拼错、参数类型不一致或漏掉 `const` 修饰符，编译器不会报错，而是将其视为一个新函数（隐藏基类函数），导致多态失效212。
**规避方案**：所有重写基类虚函数的地方，**必须**加上 `override` 关键字。

```cpp
class Base {
public:
    virtual void Process(int val) const { /*...*/ }
};
class Derived : public Base {
public:
    // 陷阱：漏了 const，或者参数类型写成了 double
    void Process(int val) { /*...*/ } // 编译通过，但并未重写！
};

// 正确做法：让编译器帮你检查
class SafeDerived : public Base {
public:
    void Process(int val) const override { /*...*/ } // 签名不匹配会直接报错
};
```

### **4. 在构造/析构函数中调用虚函数**

**陷阱描述**：在基类的构造函数或析构函数中直接或间接调用虚函数，不会触发多态（动态绑定），而是调用当前层级的版本。如果调用的是纯虚函数，会导致未定义行为（通常是运行时崩溃）。
**规避方案**：绝不在构造/析构函数中调用虚函数。如需初始化逻辑，提取为独立的非虚 `init()` 方法。

```cpp
class Base {
public:
    Base() { 
        Init(); // 危险！此时派生类尚未构造完成
    }
    virtual void Init() = 0; // 纯虚函数
};
class Derived : public Base {
public:
    void Init() override { 
        std::cout << "Derived Init\n"; 
    }
};
// 构造 Derived 时，Base 构造函数调用 Init()，会触发 "pure virtual function call" 崩溃
```

### **5. 纯虚析构函数忘记提供实现**

**陷阱描述**：将析构函数声明为纯虚（`= 0`）可以使类成为抽象类，但与普通纯虚函数不同，**纯虚析构函数必须提供函数体**。否则派生类对象在销毁时会因找不到基类析构函数而引发链接错误。
**规避方案**：声明纯虚析构后，务必在类内或类外提供空实现。

```cpp
class IInterface {
public:
    virtual ~IInterface() = 0; // 纯虚析构
};

// 必须提供实现，否则链接报错
IInterface::~IInterface() {} 
```

### **6. 滥用** `final` **关键字**

**陷阱描述**：

1. 将 `final` 用于非虚函数：会导致编译错误，`final` 只能用于虚函数或类。
2. 过早对类或虚函数使用 `final`：虽然可能带来微小的性能优化（Devirtualization），但会破坏代码的扩展性，例如导致无法为单元测试创建 Mock 对象。

**规避方案**：仅在明确设计意图（如安全限制、不可变核心逻辑）时使用 `final`。

```cpp
class Base {
public:
    // 错误：final 不能用于非虚函数
    // void NormalFunc() final {} 
    
    // 正确：锁定虚函数实现，禁止子类再修改
    virtual void CoreLogic() final { /*...*/ }
};

// 正确：禁止该类被继承
class SealedClass final { 
    // ... 
};
```

这份清单可以作为日常 Code Review 的检查项。在实际工程中，牢记**“基类析构必虚”、“重写必加 override”、“传参必用指针/引用”**这三条黄金法则，能避开 90% 的多态陷阱。