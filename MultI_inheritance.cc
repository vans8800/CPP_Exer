#include <iostream> 

using namespace std; 
  
class Animal 
{ 
  public: 
     Animal() 
     { 
        cout << "Animal constructor" << endl; 
     } 
 
    // sample member 
    int age; 
}; 
 
// virtual inheritance ensures that there is only one instance of the base class (Animal) in the derived class (Platypus), 
// even though it inherits from multiple classes (Mammal, Bird, Reptile) that themselves inherit from Animal. 
// This prevents the "diamond problem" in multiple inheritance scenarios.
class Mammal:public virtual Animal {};
class Bird:public virtual Animal  {};
class Reptile:public virtual Animal {};

class Platypus final:public Mammal, public Bird, public Reptile 
{ 
    public: 
        Platypus() 
        { 
         cout << "Platypus constructor" << endl; 
        } 
}; 

int main() 
{ 
    Platypus duckBilledP; 
    // no compile error as there is only one Animal::age 
    duckBilledP.age = 25; 
    return 0; 
} 