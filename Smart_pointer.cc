// C++ 智能指针是包含运算符重载的类，其行为类似于指针。
// 智能指针的主要目的是管理动态分配的对象的生命周期，确保在不再需要时自动释放内存，从而防止内存泄漏和悬挂指针问题。
// 常见的智能指针类型包括 `std::unique_ptr`、`std::shared_ptr` 和 `std::weak_ptr`，
// 它们分别提供了不同的所有权和引用计数机制，以适应不同的使用场景。


#include <iostream> 
#include <memory> // include this to use std::unique_ptr 
using namespace std; 


template <typename T> 
class smart_pointer 
{ 
private: 
    T* rawPtr; 
public: 
    smart_pointer (T* pData) : rawPtr(pData) {} // constructor 
    ~smart_pointer () {delete rawPtr;};         // destructor 
    // copy constructor 
    smart_pointer (const smart_pointer & anotherSP); 
    // copy assignment operator 
    smart_pointer& operator= (const smart_pointer& anotherSP); 

    // 重载解引用运算符和成员选择运算符，使智能指针的使用方式与普通指针类似。
    T& operator* () const // dereferencing operator 
    { 
        return *(rawPtr); 
    } 
 
    T* operator-> () const // member selection operator 
    { 
        return rawPtr; 
    } 
};

//使智能指针真正“智能”的是复制构造函数、赋值运算符和析构函数的实现，它们决定了智能指针对象被传递给函数、赋值或离开作用域（即像其他类对象一样被销毁）时的行为。
//介绍完整的智能指针实现前，需要了解一些智能指针类型。
// 内存资源管理(即实现 RAII)是智能指针的核心功能。
// 智能指针通过在构造函数中获取资源，在析构函数中释放资源，确保资源的正确管理和释放，从而避免内存泄漏和悬挂指针问题。
// 智能指针决定在复制和赋值时如何处理资源的所有权和引用计数，这对于确保资源的正确管理和避免内存泄漏至关重要。
// 智能指针的分类就是内存资源管理策略的分类
// 深复制、写时复制、引用计数、引用链接、破坏性复制
// 智能指针的分类，主要有以下几种：
// 1. 独占所有权智能指针（Unique Ownership Smart Pointer）：
//   - 代表：`std::unique_ptr`
//   - 特点：独占所管理的对象，不能被复制，只能移动。
//   - 使用场景：适用于需要明确所有权的情况，确保对象在离开作用域时被正确释放。
// 2. 共享所有权智能指针（Shared Ownership Smart Pointer）：    
//   - 代表：`std::shared_ptr`
//   - 特点：允许多个智能指针共享同一个对象，通过引用计数来管理对象的生命周期。
//   - 使用场景：适用于需要多个所有者的情况，确保对象在最后一个所有者离开作用域时被正确释放。       
// 3. 弱引用智能指针（Weak Reference Smart Pointer）：
//   - 代表：`std::weak_ptr`        
//   - 特点：不拥有所管理的对象，不会增加引用计数，通常与 `std::shared_ptr` 配合使用。
//   - 使用场景：适用于需要观察对象但不影响其生命周期的情况，避免循环引用导致的内存泄漏。


class Fish 
{ 
public: 
    Fish() {cout << "Fish: Constructed!" << endl;} 
    ~Fish() {cout << "Fish: Destructed!" << endl;} 

    void Swim() const {cout << "Fish swims in water" << endl;} 
};  

void MakeFishSwim(const unique_ptr<Fish>& inFish) 
{ 
    inFish->Swim(); 
} 

int main() 
{ 
    unique_ptr<Fish> smartFish (new Fish); 
 
    smartFish->Swim(); 
    MakeFishSwim(smartFish); // OK, as MakeFishSwim accepts reference 

    unique_ptr<Fish> copySmartFish; 
    //copySmartFish = smartFish; // error: operator= is private 

    copySmartFish = move(smartFish); // OK, as move transfers ownership
    if (!smartFish) 
    { 
        cout << "smartFish is empty after move" << endl; 
    }
    return 0; 
} 