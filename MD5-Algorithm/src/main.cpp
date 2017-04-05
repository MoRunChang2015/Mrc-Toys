#include <iostream>
#include <string>
#include "MD5.hpp"
using namespace std;
int main() {
    cout << "Please input your plaintext: ";
    string str;
    while (getline(cin, str)) {
        cout << "Encrypted string: " << MD5::encrypt(str) << endl;
        cout << "Please input your plaintext: ";
    }
    return 0;
}
