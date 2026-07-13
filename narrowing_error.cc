#include <iostream>


using namespace std;

int main(void)
{

	int largeNum = 5000000; 
	// short anotherNum1{ largeNum }; // error! Amend types 
	int anotherNum2{ largeNum }; // OK! 
	// float someFloat1{ largeNum }; // error! An int may be narrowed 
	float someFloat2{ 5000000 }; // OK! 5000000 can be accomodated 

}

//g++ -c narrowing_err.cc
