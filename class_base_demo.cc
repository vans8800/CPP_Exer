#include <iostream> 

using namespace std; 
 
class Fish 
{ 

    protected: 
        bool isFreshWaterFish;
    public: 
        Fish(bool isFreshWater): isFreshWaterFish(isFreshWater) {}
    
        void Swim() 
        { 
           if (isFreshWaterFish) 
                 cout << "Swims in lake" << endl; 
           else 
                 cout << "Swims in sea" << endl; 
        } 
}; 
 
class Tuna: public Fish 
{ 
    public: 
        Tuna(): Fish(false){} 
   
    void swim() 
    { 
        cout << "Tuna Swims real fast " << endl; 
    }
}; 
 

class Carp: public Fish 
{ 
    public: 
        Carp() : Fish(true) {}
    
    void Swim() 
    { 
        cout << "Carp swims real slow" << endl; 
    }
}; 
 

int main() 
{  
        Carp myLunch; 
        Tuna myDinner; 
    
        cout << "About my food:" << endl; 
    
        cout << "Lunch: "; 
        myLunch.Swim(); 
   
        cout << "Dinner: "; 
        myDinner.Swim(); 

        // Call the base class method explicitly 
        // 调用基类中被覆盖的方法
        myDinner.Fish::Swim();

        // Fish::Swim(); // 调用基类中被覆盖的方法
        return 0; 
} 