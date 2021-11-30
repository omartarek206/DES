#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string a ="";
    string b = "abcd";

    for (int i = 0; i < b.length(); i++) {
        a += b[i];
        cout << a << "-----" << b << endl;
    }
}