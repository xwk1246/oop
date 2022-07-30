#include <iostream>
using namespace std;

int main() {
    int cnt = 0;
    int j;
    for (int i = 0; i < 3; i++) {
        for (j = 2; j < i; j++) {
            if (i % j == 0)break;
        }
        if (j == i - 1)cnt++;
    }
    cout << cnt;
    return 0;
}