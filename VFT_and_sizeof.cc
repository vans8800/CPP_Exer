#include <iostream> 
using namespace std; 

class SimpleClass 
{ 
   int a, b; 
 
   public: 
        void DoSomething() {} 
    }; 
 
 class Base 
{ 
    int a, b; 
 
 public: 
    virtual void DoSomething() {} 
 }; 
 

int main() 
 { 
    // 2 * sizeof(int) = 8
    cout << "sizeof(SimpleClass) = " << sizeof(SimpleClass) << endl; 
    
    // 2 * sizeof(int) + 1 * sizeof(void*) = 8 + 8 = 16
    //VFT: The size of a pointer is typically 8 bytes on a 64-bit system

    //添加关键字 virtual 带来的影响是，编译器将为 Base 类生成一个虚函数表，并其虚函数表指针（一个隐藏成员） 预留空间
    cout << "sizeof(Base) = " << sizeof(Base) << endl; 
    cout << "sizeof(int) = " << sizeof(int) << endl;
    return 0; 
 } 