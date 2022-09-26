#include <iostream>

int main() {
    int* a = nullptr;
    a = new int(4);
    a[0] = 1; a[1] = 101; a[2] = 44; a[3] = 3; 
    std::cout << a[0] << "\n";
    delete[] a;
    std::cout << a << "\n";
    a = nullptr;
    std::cout << a[0] << "\n";
    std::cout << "Test";
    return 0;
}