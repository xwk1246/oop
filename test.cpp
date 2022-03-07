#include <queue>
#include <vector>
#include <iostream>
using namespace std;

class pqLink {
public:
    int from;
    int to;
    int weight;
};

class compare {
public:
    bool operator()(pqLink a, pqLink b) {
        return a.weight > b.weight;
    }
};

int main() {
    priority_queue<pqLink, vector<pqLink>, compare>pq;
    pq.push({ 0,0,0 });
    pq.push({ 0,0,1 });
    pq.push({ 0,0,2 });
    while (!pq.empty()) {
        cout << pq.top().weight << endl;
        pq.pop();
    }
    return 0;
}