#include <iostream> 
#include <sstream> // new include for ostringstream 
#include <string> 
using namespace std; 


/* static_cast example */
//""static_cast 用于在相关类型的指针之间进行转换，还可显式地执行标准数据类型的类型转换—这种转换原本将自动或隐式地进行。
// 用于指针时，static_cast 实现了基本的编译阶段检查，确保指针被转换为相关类型。
// 这改进了 C 风格类型转换，在 C 语言中，可将指向一个对象的指针转换为完全不相关的类型，而编译器不会报错。
// 使用 static_cast 可将指针向上转换为基类类型，也可向下转换为派生类型""
class Date 
{ 
    private: 
        int day, month, year; 
        string dateInString; 
    
    public: 
        Date(int inMonth, int inDay, int inYear) 
           : month(inMonth), day(inDay), year(inYear) {}; 
 

    //对于使用关键字 explicit 声明的转换运算符和构造函数，要使用它们，也必须通过 static_cast
    explicit operator const char*() 
    { 
       ostringstream formattedDate; // assists string construction 
       formattedDate << month << " / " << day << " / " << year; 
 
       dateInString = formattedDate.str(); 
       return dateInString.c_str(); 
    } 
}; 


int main () 
{ 
    Date Holiday (12, 25, 2016); 
 
    // Use static_cast to convert Holiday to const char* and print it explicitly
    cout << "Holiday is on: " << static_cast<const char*>(Holiday) << endl; 

    //string strHoliday(static_cast<const char*>(Holiday)); 

    // strHoliday=static_cast<const char*>(Date(11,11,2016));
 
    // string strHoliday (Holiday); // OK! 
    // strHoliday = Date(11, 11, 2016); // also OK! 
 
    return 0; 
} 