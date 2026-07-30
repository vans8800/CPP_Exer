#include <iostream> 

using namespace std; 
 
template <typename Res, typename ValType> 
void Sum(Res& result, ValType& val) 
{ 
    result = result + val; 
} 
  
 // C++14 参数数量可变的模板是 2014 年发布的 C++14 新增的
template <typename Res, typename First, typename... Rest> 
void Sum(Res& result, First val1, Rest... valN) 
{ 
    cout << "参数数量：" <<sizeof...(Rest) << endl;
    result = result + val1; 
    return Sum(result, valN ...); 
} 
 
int main() 
{ 
    double dResult = 0; 
    Sum (dResult, 3.14, 4.56, 1.1111); 
    cout << "dResult = " << dResult << endl; 
 
    string strResult; 
    Sum (strResult, "Hello ", "World"); 
    cout << "strResult = " << strResult.c_str() << endl; 
 
    return 0; 
} 