//利用可变参数模板，实现元组std::tuple的打印
#include <iostream>
#include <tuple>
#include <string>

using namespace std;

template <typename... Args>
void printTuple(const Args&... args) {
    (std::cout << ... << args) << std::endl;
}

template <typename tupleType> 
void DisplayTupleInfo(tupleType& tup) 
{  
   const int numMembers = tuple_size<tupleType>::value; 
    std::cout << "Num elements in tuple: " << numMembers << std::endl; 
    std::cout << "Last element value: " << std::get<numMembers - 1>(tup) << std::endl; 
} 

void test_DisplayTupleInfo() 
{ 
    std::tuple<int, char, std::string> tup1(std::make_tuple(101, 's', "Hello Tuple!")); 
    DisplayTupleInfo(tup1); 

    auto tup2(std::make_tuple(3.14, false)); 
    DisplayTupleInfo(tup2); 


    auto concatTup(std::tuple_cat(tup2, tup1)); // contains tup2, tup1 members 
    DisplayTupleInfo(concatTup); 

    double pi; 
    std::string sentence; 
    std::tie(pi, std::ignore, std::ignore, std::ignore, sentence) = concatTup; 
    std::cout << "Unpacked! Pi: " << pi << " and \"" << sentence << "\"" << std::endl; 
} 


int main() {
    //printTuple(1, 2.5, "Hello");
    //std::cout << std::endl;

    test_DisplayTupleInfo();
    return 0;
}   