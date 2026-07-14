#include <iostream>

using namespace std;

// 动态分配int型 MxN 的二维数组，请用C++写出分配和释放语句，要求使用new和delete运算符。
template <typename T>
T** allocate2DArray(int M, int N) { 
    T** arr = new T*[M];
    for (int i = 0; i < M; i++) {
        arr[i] = new T[N];
    }
    return arr;
}

template <typename T>
void deallocate2DArray(T** arr, int M) {        
    for (int i = 0; i < M; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

template <typename T>
void print2DArray(T** arr, int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}


int main(void)
{
    int M = 3, N = 4;
    int** myArray = allocate2DArray<int>(M, N);

    // Initialize the array with some values
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            myArray[i][j] = i * N + j;
        }
    }

    // Print the array
    print2DArray(myArray, M, N);

    // Deallocate the array
    deallocate2DArray(myArray, M);

    return 0;
  
}