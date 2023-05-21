#include <iostream>
#include <cassert>

#include "Vector.h"
#include <vector>

using std::cout;
using std::endl;

int main() {
    // Test Case 1: Sukurti tuscia vektoriu ir patikrinti elementu skaiciu bei vektoriaus dydi
    cout << "Test 1: Sukuriamas tuscias vektorius\n";
    Vector<int> customVector;
    std::vector<int> stdVector;
    cout << "Custom Vector: Size = " << customVector.size() << ", Capacity = " << customVector.capacity() << "\n";
    cout << "Std Vector: Size = " << stdVector.size() << ", Capacity = " << stdVector.capacity() << "\n";

    // Test Case 2: Iterpiame naujus elementus ir patikriname vektoriaus dydi
    cout << "\nTest Case 2: Iterpiame naujus elementus\n";
    customVector.push_back(10);
    customVector.push_back(20);
    customVector.push_back(30);
    stdVector.push_back(10);
    stdVector.push_back(20);
    stdVector.push_back(30);
    cout << "Custom Vector: Size = " << customVector.size() << "\n";
    cout << "Std Vector: Size = " << stdVector.size() << "\n";

    // Test Case 3: Pasiekti vektoriaus elementus naudojant operator []
    cout << "\nTest Case 3: Pasiekiame vektoriaus elementus\n";
    cout << "Custom Vector: Element at index 1 = " << customVector[1] << "\n";
    cout << "Std Vector: Element at index 1 = " << stdVector[1] << "\n";

    // Test Case 4: Istrinti elementus is vektoriaus ir patikrinti jo dydi
    cout << "\nTest Case 4: Istriname vektoriaus elementus\n";
    customVector.erase(customVector.begin() + 1);
    stdVector.erase(stdVector.begin() + 1);
    cout << "Custom Vector: Size = " << customVector.size() << "\n";
    cout << "Std Vector: Size = " << stdVector.size() << "\n";

    // Test Case 5: Patikriname reserve() ir shrink_to_fit()
    cout << "\nTest Case 5: Tikriname reserve() bei shrink_to_fit()\n";
    customVector.reserve(100);
    stdVector.reserve(100);
    cout << "Custom Vector: Capacity = " << customVector.capacity() << "\n";
    cout << "Std Vector: Capacity = " << stdVector.capacity() << "\n";
    customVector.shrink_to_fit();
    stdVector.shrink_to_fit();
    cout << "Custom Vector: Capacity = " << customVector.capacity() << "\n";
    cout << "Std Vector: Capacity = " << stdVector.capacity() << "\n";
    return 0;
}
