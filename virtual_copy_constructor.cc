#include <iostream> 

using namespace std; 
  
class Fish 
{ 
  public: 
     virtual Fish* Clone() = 0; // pure virtual function to clone the object
     virtual void Swim() = 0; 
     virtual ~Fish() {}; 
  }; 

  
class Tuna: public Fish 
{ 
 public: 
    Fish* Clone() override // override the Clone method to return a copy of Tuna
    { 
       return new Tuna (*this); 
    } 
 
    //当 final 放在虚函数声明的末尾时，表示该虚函数在当前类中是最终版本，不能在派生类中被重写（override）
    // 禁止派生类重写该方法
    void Swim() override final //
    { 
       cout << "Tuna swims fast in the sea" << endl; 
    } 
}; 
 
class BluefinTuna final:public Tuna 
{ 
    public: 
        Fish* Clone() override 
        { 
         // 模拟复制构造函数的行为，返回一个新的 BluefinTuna 对象
           return new BluefinTuna(*this); 
        } 
 
    // Cannot override Tuna::Swim as it is "final" in Tuna 
}; 
 
class Carp final: public Fish 
{ 
    Fish* Clone() override 
    { 
       return new Carp(*this); 
    } 
    void Swim() override final 
    { 
       cout << "Carp swims slow in the lake" << endl; 
    } 
}; 
 

int main() 
{ 
    const int ARRAY_SIZE = 4; 
 
    Fish* myFishes[ARRAY_SIZE] = {NULL}; 
    // create an array of Fish pointers and initialize them with different derived class objects
    myFishes[0] = new Tuna(); 
    myFishes[1] = new Carp(); 
    myFishes[2] = new BluefinTuna(); 
    myFishes[3] = new Carp(); 
 
    Fish* myNewFishes[ARRAY_SIZE]; 
    for (int index = 0; index < ARRAY_SIZE; ++index) 
       myNewFishes[index] = myFishes[index]->Clone(); 
 
    // invoke a virtual method to check 
    for (int index = 0; index < ARRAY_SIZE; ++index) 
       myNewFishes[index]->Swim(); 
 
    // memory cleanup 
    for (int index = 0; index < ARRAY_SIZE; ++index) 
    { 
       delete myFishes[index]; 
        delete myNewFishes[index]; 
    } 
 
    return 0; 
} 