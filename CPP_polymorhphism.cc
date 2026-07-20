#include <iostream> 

using namespace std; 

class Fish 
{ 
    public: 
     Fish() 
     { 
        cout << "Constructed Fish" << endl; 
     } 
   virtual ~Fish() // virtual destructor to ensure proper cleanup of derived class objects
    { 
       cout << "Destroyed Fish" << endl; 
     } 
 }; 
 
class Tuna:public Fish 
{ 
    public: 
    Tuna() 
    { 
       cout << "Constructed Tuna" << endl; 
    } 
    ~Tuna() 
    { 
       cout << "Destroyed Tuna" << endl; 
    } 
 }; 
 
void DeleteFishMemory(Fish* pFish) 
{ 
    delete pFish; 
} 
int main() 
{ 
    cout << "Allocating a Tuna on the free store:" << endl; 
    Tuna* pTuna = new Tuna; 

    cout << "Deleting the Tuna: " << endl; 
    DeleteFishMemory(pTuna); 
    
    cout << endl;
    cout << "Instantiating a Tuna on the stack:" << endl; 
    Tuna myDinner; 
    
    cout << "Automatic destruction as it goes out of scope: " << endl; 
    return 0; 
} 
  
// class Fish 
// { 
//     public: 
//        virtual void Swim() // virtual function override by derived class 
//         { 
//            cout << "Fish swims! " << endl; 
//         } 
        
//         // 虚析构函数（良好习惯，防止通过基类指针删除派生类对象时发生内存泄漏）
//        virtual ~Fish() = default; 
// };

// class Tuna:public Fish 
// { 
//     public: 
//         // override Fish::Swim 
//         void Swim() 
//         { 
//            cout << "Tuna swims!" << endl; 
//         } 
// }; 

// // 避免对象切片问题，使用引用传递参数
// // 基类指针或引用方式

// void MakeFishSwim(Fish& inputFish) 
// { 
//     // calling Fish::Swim 
//     inputFish.Swim(); 
// } 

// int main() 
// { 
//     Tuna myDinner; 
   
//     // calling Tuna::Swim 
//     myDinner.Swim(); 
   
//     // sending Tuna as Fish 
//     MakeFishSwim(myDinner); 
   
//     return 0; 
// } 