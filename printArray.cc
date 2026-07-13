#include <iostream>

using namespace std;

template <typename T, std::size_t N >
void DisplayArray(const T (&array)[N])
{
	for(const auto& elem : array){
		std::cout << elem << " " ;
	}
	std::cout << std::endl;
}

int main(void)
{
       int myNums[4] = {24, 58, -1, 245}; 
       DisplayArray(myNums); 
 
       char myStatement[7] = {'H', 'e', 'l', 'l', 'o', '!', '\0'}; 
       DisplayArray(myStatement); 
  
       return 0; 
}
