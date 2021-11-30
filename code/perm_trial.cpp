#include <bits/stdc++.h>
 
using namespace std;

vector <int> a = {1, 2, 3, 4, 5, 6};
vector <int> b = {10, 20, 30, 40, 50, 60};
vector <int> c = {11, 12, 13, 14, 15, 16};

map<string, vector<int>*> tables = {
    {"aa", &a},
    {"bb", &b},
    {"cc", &c}
};

int main() {
    vector<int> aa = *tables["aa"];
    vector<int> bb = *tables["bb"];
    vector<int> cc = *tables["cc"];
    
    for (int i = 0; i < 6; i++) {
        cout << a[i] << " " << b[i] << " " << c[i] << endl;
    }
    cout << "////////////////////////////////////////////////////////////////////////////////" << endl;
    for (int i = 0; i < 6; i++) {
        cout << aa[i] << " " << bb[i] << " " << cc[i] << endl;
    }
    return 0;
}