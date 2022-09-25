#include <iostream>
#include "poly.h"

using namespace std;

int main() {

    Poly p1(2);
    cin >> p1;    
    cout << p1;
    p1.salvar("poly1.txt");
    Poly p2(2);
    cout << p2;
    p2.salvar("poly2.txt");
    Poly p3(6);
    cout << p3;
    Poly p4(-1);
    cout << p4;
    // Poly p5(3);


    p3.recriar(10);
    cout << "\nMain";
    cout << p1.empty();
    cout << p3.isZero();
    // cout << "\nMain";
    return 0;
}