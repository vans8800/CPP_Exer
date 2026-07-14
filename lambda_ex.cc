#include <iostream> 
#include <algorithm> 
#include <vector> 

using namespace std; 
  
void DisplayNums(vector<int>& dynArray) 
{ 
      for_each (dynArray.begin(), dynArray.end(), \
		      [](int Element) {cout << Element << " ";} ); 
      cout << endl; 
} 
 
int main() 
{ 
    vector<int> myNums; 
    myNums.push_back(501); 
    myNums.push_back(-1); 
    myNums.push_back(25); 
    myNums.push_back(-35); 
 
    DisplayNums(myNums); 
 
    cout << "Sorting them in descending order" << endl; 
 

    // 这里使用了一个lambda表达式来定义排序规则，返回值为bool类型，表示Num2是否小于Num1，从而实现降序排序
    // lambda表达式的语法为：[](参数列表) {函数体}，其中[]表示捕获外部变量的方式，这里没有捕获任何外部变量，所以是空的
    // [optional parameters](parameter list){ statements; } 
    sort (myNums.begin(), myNums.end(),\
		    [](int Num1, int Num2) {return (Num2 < Num1); } ); 
 
    DisplayNums(myNums); 
    return 0; 
} 





